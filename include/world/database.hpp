#ifndef ENNOVIA_DATABASE_HPP
#define ENNOVIA_DATABASE_HPP

#include <sstream>
#include <boost/shared_ptr.hpp>


namespace Ennovia {

    class Database {
    public:
        typedef boost::shared_ptr<MYSQL_RES> Result;

        Database(const std::string& ip, const std::string& username, const std::string& password);
        ~Database();

        int playerLogin(const std::string& username, const std::string& password);
    private:
        std::stringstream qs;
        void query();
        Result result();
        std::string escape(const std::string& str);
        PgDatabase db;
    };
}

#endif
