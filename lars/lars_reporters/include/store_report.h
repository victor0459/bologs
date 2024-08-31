#include "lars.pb.h"
#include"mysql.h"
#include "lars_reactor.h"
class StoreReport{
	public:
		StoreReport();

		void store(lars::ReportStatusRequest &req);
	private:
		MYSQL _db_conn;//数据库连接


};
void *store_main(void*);
