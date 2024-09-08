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
void SubcribeList::make_publish_map(listen_fd_set&online_fds,publish_map&need_publish)
{
	publish_map::iterator it;
	pthread_mutex_lock(&_push_list_lock);

	for(it=_push_list.begin();it!=_push_list.end();it++){
		if(online_fds.find(it->first)!=online_fds.end()){
			hash_set<uint64_t>::iterator st;
			for(st=_push_list[it->first].begin();st!=_push_list[it->first].end();it++){

				need_pushlish[it->first].insert(*st);


			}

		}



	}

	pthread_mutex_unlock(&_push_list_lock);


}



