#include <iostream>
#include "Node.h"
#include "skipList.h"
using namespace std;

int main() {

    skipList<int, int> sl;
    int length = 10;

    for (int i = 1; i <= length; ++i) {
        sl.insert(i, i + 200);
    }

    cout << "The number of elements in skiplist is:" << sl.size() << endl;

    if (sl.size() != length) {
        cout << "insert failur." << endl;
    } else {
        cout << "insert success." << endl;
    }

    //测试查找
    int value = -1;
    int key = 9;
    Node<int, int> *searchResult = sl.find(key);
    if (searchResult != nullptr) {
        value = searchResult->getValue();
        cout << "search result for key " << key << ":" << value << endl;
    } else {
        cout << "search failure for key " << key << endl;
    }


    value = -1;

    //测试移除,测试不通过
    key = 6;
    cout<<endl<<"start to remove"<<endl;
    bool removeResult = sl.remove(key, value);
    if (removeResult) {
        cout << "removed node whose key is " << key << " and value is " << value << endl;
    } else {
        cout << "removed failure" << endl;
    }

    return 0;
}
