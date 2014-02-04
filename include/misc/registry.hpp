#ifndef ENNOVIA_REGISTRY_HPP
#define ENNOVIA_REGISTRY_HPP

#include <map>
#include <cstdlib>
#include <cstdio>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bimap.hpp>
#include "misc/registry_forw.hpp"

namespace Ennovia
{

template <class X>
struct Deleter {
    static void run(X x) {
        delete(x);
    }
};

template <class T>
class Identificator
{
public:
};


template <>
class Identificator<int>
{
public:
    static int generate() {
        return std::rand();
    }

    static int defaultV() {
        return 0;
    }

    static bool compare(int x1,int x2) {
        return x1==x2;
    }
};

template <>
class Identificator<void*>
{
public:
    static void* defaultV() {
        return (void*)0;
    }

    static bool compare(void* x1, void* x2) {
        return x1 == x2;
    }

};


template <>
class Identificator<std::string>
{
public:
    static std::string defaultV() {
        return std::string();
    }

    static bool compare(const std::string& x1,const std::string& x2) {
        return x1.compare(x2) == 0;
    }

};


template <class Type_, class I = int>
class Registry
{
public:
    typedef I Identifier;
    typedef Identificator<I> ID;
    typedef Type_ Type;

    Registry() {
        std::cout << "Registry created" << std::endl;
    }


    typedef boost::bimap<I,Type> map_type;
    map_type map;

    void erase(I id)
    {
        Deleter<Type>::run(map.left.at(id));
        //delete();
        map.left.erase(id);
    };


    Type value(I id)
    {
        return map.left.at(id);
    };

    bool exists(I id)
    {
        return map.left.find(id) != map.left.end() && value(id) != 0;
    };

    bool exists(const Type& v) {
        return map.right.find(v)!=map.right.end();
    }

    I id(const Type& v)
    {
        return map.right.at(id);
    }

    I insert(const Type& v)
    {
        typename map_type::right_const_iterator i = map.right.find(v);
        if(i != map.right.end())
        {
            return i->second;
        }
        I id;
        do
        {
            id = ID::generate();
        } while(Identificator<I>::compare(id,Identificator<I>::defaultV()) || exists(id));
        insert(v,id);
        return id;
    }

    bool insert(const Type& v, I id) {
        bool existed = exists(id);
        bool newValue = existed && value(id) != v;
        if(newValue) erase(id);
        if(!existed || newValue) map.insert(typename map_type::value_type(id,v));
        return existed;
    }

    static Registry<Type,I>& get()
    {
        static Registry<Type,I> registry;
        return registry;
    }

};


template <class X, class I>
class reg
{
public:
    typedef Registry<X*,I> reg_type;

    reg() : id_(Identificator<I>::defaultV()) {}

    reg(X* x) : id_(reg_type::get().insert(x))
    {
    }

    reg(X* x, I id) : id_(id) {
        reg_type::get().insert(x,id);
    }

    reg(I id) : id_(id) {
    }

    reg(const reg<X,I>& reg2) : id_(reg2.id_)
    {
    }

    reg<X,I>& operator=(const reg<X,I>& r) {
        id_ = r.id_;
        return *this;
    }

    static bool exists(X* x) { return reg_type::get().exists(x); }

    bool operator==(const reg<X,I>& r) {
        return Identificator<I>::compare(id_,r.id_);
    }

    void erase() {
        reg_type::get().erase(id_);
    }

    bool exists()
    {
        return reg_type::get().exists(id_);
    }


    I id()
    {
        return id_;
    }

    X* get()
    {
        return reg_type::get().value(id_);
    }
    const X* get() const
    {
        return reg_type::get().value(id_);
    }
    X& operator *()
    {
        return *get();
    }
    const X& operator *() const
    {
        return *get();
    }
    X* operator ->()
    {
        return get();
    }
    const X* operator ->() const
    {
        return get();
    }

    operator X*()
    {
        return get();
    }
private:
    I id_;
};
}
#endif
