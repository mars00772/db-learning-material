#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
//#include <shared_mutex>
//#include <mutex>

class LRU{
public:

    explicit LRU(int capacity){
        this->capacity = capacity;
    }
    ~LRU()= default;

    const std::string get(std::string &key){
        //std::shared_lock<std::shared_mutex> lock(mutex);//析构会自动解锁
        if(!map.count(key)){
            return "";
        }
        update(key);
        return map[key];
    }

    void put(const std::string& key, const std::string& value){
        //std::unique_lock<std::shared_mutex> lock(mutex);
        if(this->capacity==map.size()&&!map.count(key)){
            evict();
        }
        update(key);
        map[key] = value;
    }

    void remove(const std::string& key){
        //std::unique_lock<std::shared_mutex> lock(mutex);
        if(map.count(key)){
            lru.erase(itermp[key]);
            map.erase(key);
            itermp.erase(key);
        }

    }
    void printAll(){
        //std::shared_lock<std::shared_mutex> lock(mutex);//析构会自动解锁
        for(auto iter = map.begin(); iter != map.end(); iter++ ){
            std::cout<< iter->first << " : " << iter->second << std::endl;
        }
    }

private:
    int capacity;
    //mutable std::shared_mutex mutex;
    std::list<std::string> lru;
    std::unordered_map<std::string, std::string> map;
    std::unordered_map<std::string, std::list<std::string>::iterator> itermp;

    void update(const std::string& key){
        if(map.count(key)){
            lru.erase(itermp[key]);
        }
        lru.push_front(key);
        itermp[key] = lru.begin();
    }

    void evict(){
        map.erase(lru.back());
        itermp.erase(lru.back());
        lru.pop_back();
    }
};

int main() {

    std::unique_ptr<LRU> lru(new LRU(3));
    lru->put("key1", "value1");
    lru->put("key2", "value2");
    lru->put("key3", "value3");

    //淘汰的应该是key1
    lru->put("key4", "value4");

    /** result:
     *  key2 : value2
        key3 : value3
        key4 : value4
     */
    lru->printAll();

    std::cout<<std::endl;
    lru->remove("key3");
    lru->put("key5", "value5");

    /**
     *  key2 : value2
        key4 : value4
        key5 : value5
     */
    lru->printAll();

    std::cout<<std::endl;
    lru->put("key4", "newValue4");

    //淘汰的应该是key2
    lru->put("key6", "value6");

    /***
     * key4 : newValue4
        key5 : value5
        key6 : value6
     */
    lru->printAll();

}

