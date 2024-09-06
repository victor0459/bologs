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
