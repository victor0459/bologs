#include "subcribe.h"
#include "lars.pb.h"

extern tcp_server*server;
SubcribeList*SubcribeList:: instance=NULL;
pthread_once_t SubcribeList::_once=PTHREAD_ONCE_INIT;
SubcribeList::SubcribeList()
{
	pthread_mutex_init(&_book_list_lock,NULL);
	pthread_mutex_init(&_push_list_lock,NULL);


}


void SubcibeList::subcribe(uint64_t mod,int fd)
{
	pthread_mutex_lock(&_book_list_lock);
	_book_list[mod].insert(fd);
	pthread_mutex_unlock(&_book_list_lock);


}
void SubcribeList::unsubcribe(uint64_t mod,int fd)
{
	pthread_mutex_lock(&_book_list_lock);
	if(_book_list.find(mod)!=_book_list.end()){
		_book_list[mod].erase(fd);
		if(_book_list[mod].empty()==true){
			_book_list.earse(mod);
		}
	}
	pthread_mutex_unlock(&_book_list_lock);
}




