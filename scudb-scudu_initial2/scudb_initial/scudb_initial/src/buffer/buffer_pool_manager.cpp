#include "buffer/buffer_pool_manager.h"
#include "common/logger.h"

namespace scudb {

BufferPoolManager::BufferPoolManager(size_t pool_size,
                                                 DiskManager *disk_manager,
                                                 LogManager *log_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager),
      log_manager_(log_manager) {

  pages_ = new Page[pool_size_];
  page_table_ = new ExtendibleHash<page_id_t, Page *>(BUCKET_SIZE);
  replacer_ = new LRUReplacer<Page *>;
  free_list_ = new std::list<Page *>;


  for (size_t i = 0; i < pool_size_; ++i) {
    free_list_->push_back(&pages_[i]);
  }
}

BufferPoolManager::~BufferPoolManager() {
  delete[] pages_;
  delete page_table_;
  delete replacer_;
  delete free_list_;
}


Page *BufferPoolManager::FetchPage(page_id_t page_id)
{
	assert(page_id != INVALID_PAGE_ID);
	std::lock_guard<std::mutex> lock(mutex);

	Page *res = nullptr;
	if (page_table_->Find(page_id, res))
	{
		++res->pin_count_;
		replacer_->Erase(res);
		return res;
	}
	else
	{
		if (!free_list_->empty())
		{
			res = free_list_->front();
			free_list_->pop_front();
		}
		else
		{
			if (!replacer_->Victim(res))
			{
				return nullptr;
			}
		}
	}

	assert(res->pin_count_ == 0);
	if (res->is_dirty_)
	{
		disk_manager_->WritePage(res->page_id_, res->GetData());
	}
	page_table_->Remove(res->page_id_);

	page_table_->Insert(page_id, res);

	res->page_id_ = page_id;
	res->is_dirty_ = false;
	res->pin_count_ = 1;
	disk_manager_->ReadPage(page_id, res->GetData());

	return res;
}

bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty) {
    std::lock_guard<std::mutex> guard(mutex);

    Page *page = nullptr;
    if (!page_table_->Find(page_id, page)) {
        return false;
    }
    if (page->pin_count_ == 0) {
        return false;
    }
    page->pin_count_--;
    if (page->pin_count_ == 0) {
        replacer_->Insert(page);
    }
    if (is_dirty) {
        page->is_dirty_ = true;
    }
    return true;
}

bool BufferPoolManager::FlushPage(page_id_t page_id) {
    std::lock_guard<std::mutex> guard(mutex);
    Page *page = nullptr;
    if (!page_table_->Find(page_id, page)) {
        return false;
    }
    if (page->is_dirty_) {
        disk_manager_->WritePage(page_id, page->GetData());
        page->is_dirty_ = false;
    }
    return true;
}

bool BufferPoolManager::DeletePage(page_id_t page_id) {
    std::lock_guard<std::mutex> guard(mutex);
    Page *page = nullptr;
    if (page_table_->Find(page_id, page)) {
        if (page->GetPinCount() != 0) {
            return false;
        }
        page->page_id_ = INVALID_PAGE_ID;
        page->pin_count_ = 0;
        page->is_dirty_ = false;
        page->ResetMemory();

        replacer_->Erase(page);
        page_table_->Remove(page_id);
        free_list_->push_back(page);
    }

    disk_manager_->DeallocatePage(page_id);
    return true;
}

Page *BufferPoolManager::NewPage(page_id_t &page_id)
{
	std::lock_guard<std::mutex> lock(mutex);
	Page *res = nullptr;
	if(!free_list_->empty())
	{
		res = free_list_->front();
		free_list_->pop_front();
	}
	else
	{
		if(!replacer_->Victim(res))
		{
			return nullptr;
		}
	}
	page_id = disk_manager_->AllocatePage();
	if(res->is_dirty_)
	{
		disk_manager_->WritePage(res->page_id_, res->GetData());
	}
	page_table_->Remove(res->page_id_);
	page_table_->Insert(page_id, res);
	res->page_id_ = page_id;
	res->is_dirty_ = false;
	res->pin_count_ = 1;
	res->ResetMemory();
	return res;
}

}
