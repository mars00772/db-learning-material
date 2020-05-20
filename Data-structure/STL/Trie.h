//
// Created by AlexanderChiu on 2020/5/20.
//

#ifndef STL_TRIE_H
#define STL_TRIE_H

#include <vector>


class Trie {

private:
    struct Node;
    Node* root;
    static const int SIZE = 26;
    struct Node {
        bool isWord;
        std::vector<Node*> next;
        explicit Node():isWord{false},next(SIZE, nullptr){}
        ~Node(){
        }
    };


public:

    explicit Trie():root(new Node){
    }
    ~Trie(){
        clear();
    }
    void clear(){
        clear(root);
        root = new Node();
    }

    Trie(const Trie&) = delete;
    Trie&operator=(const Trie&) = delete;


public:

    void insert(const std::string &word){
        insert(root, word, 0);
    }

    bool contains(const std::string &word)const{
        Node* result = get(word);
        return result!= nullptr;
    }

    Node* get(const std::string &word)const{
        return get(root, word, 0);
    }

    std::vector<std::string> wordsWithPrefix(const std::string& prefix) const{
        std::vector<std::string> result;
        Node* ptr = get(root, prefix, 0);
        collect(ptr, prefix, result);
        return result;
    }

private:

    void collect(Node* root, const std::string&prefix, std::vector<std::string>&result) const{

        if(!root){
            return;
        }
        if(root->isWord){
            result.push_back(prefix);
        }
        char c = 'a';
        for(Node* &node: root->next){
            collect(node, prefix + c, result);
            c++;
        }

    }

    void insert(Node* &root, const std::string& word, int idx){

        if(!root){
            root = new Node();
        }

        if(idx==word.size()){
            root->isWord=true;
            return;
        }
        char index = word[idx];
        insert(root->next[index-'a'], word, idx+1);

    }

    Node* get(Node* root, const std::string& word, int idx) const{

        if(!root){
            return nullptr;
        }

        if(idx==word.size()){
            return root;
        }

        char index = word[idx];

        return get(root->next[index-'a'], word, idx+1);

    }

    void clear(Node* node){

        if(node){
            for(auto n:node->next){
                clear(n);
            }
            delete node;
        }
    }

};





#endif //STL_TRIE_H
