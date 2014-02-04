#include "world/database.hpp"

namespace Ennovia {

        Database::Database(const std::string& ip, const std::string& username, const std::string& password)
        : con(mysql_init(NULL)) {
            int timeout = 1000;
            bool reconnect = 1;
            mysql_options(con,MYSQL_OPT_CONNECT_TIMEOUT,&timeout);
            mysql_options(con,MYSQL_OPT_RECONNECT,&reconnect);
            if(mysql_real_connect(con,ip.c_str(),username.c_str(),password.c_str(),0,3306,0,0)) {
                std::cout << "MySQL: Failed to connect: " << mysql_error(con) << std::endl;
            }
            playerLogin("admin","lol");
        }

        Database::~Database() {
            mysql_close(con);
        }

        int Database::playerLogin(const std::string& username, const std::string& password) {
            qs << "SELECT `id` FROM `ennovia`.`player` WHERE `username`='" << escape(username) << "' AND `password`='" << escape(password) << "'";
            query();
            MYSQL_RES* r = mysql_store_result(con);
            MYSQL_ROW row;
            if(row = mysql_fetch_row(r)) {
                std::cout << "Query result: " << row[0] << std::endl;
            }
            mysql_free_result(r);
        }

        void Database::query() {
            std::cout << "Query: " << qs.str() << std::endl;
            if(mysql_query(con,qs.str().c_str())) {
                std::cout << "MySQL: Failed to query: " << mysql_error(con) << std::endl;
            }
            // clear stream
            qs.str(std::string());
        }

        Database::Result Database::result() {
            return Result(mysql_store_result(con),&mysql_free_result);
        }

        std::string Database::escape(const std::string& str) {
            char* out = new char[str.size()*2+1];
            mysql_real_escape_string(con,out,str.c_str(),str.size());
            std::string sout(out);
            delete out;
            return sout;
        }

}
