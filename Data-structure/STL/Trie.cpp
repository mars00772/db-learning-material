//
// Created by AlexanderChiu on 2020/5/20.
//

#include <iostream>
#include <string>
#include "Trie.h"


int main() {
    Trie* trie = new Trie();

    trie->insert("damon");

    trie->insert("alex");

    std::cout<<trie->contains("damon")<<std::endl;
    std::vector<std::string>keys = trie->wordsWithPrefix("da");
    for(std::string& str:keys){
        std::cout<<str<<std::endl;
    }

    trie->insert("damons");
    keys = trie->wordsWithPrefix("da");
    for(std::string& str:keys){
        std::cout<<str<<std::endl;
    }


    delete trie;
    return 0;
}