#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <sstream>

#define CACHE_SIZE 1

// enum 사용 불가 규칙 때문에, 상수 선언
#define TYPE_INT 0
#define TYPE_DOUBLE 1
#define TYPE_NODE_POINTER 2

class Cache {
private:
  // TODO: private inner struct/class 선언 가능
  // TODO: private 멤버 변수와 함수 추가 가능
  
  struct Entry{
    std::string key;
    int type;
    void* value;
  };
  
  struct Node{
    Entry* entry;
    Node* prev;
    Node* next;
  };  

  class LinkedList {
  private:
    int _size;
    Node* _head;
    Node* _tail;
  
  public:
    LinkedList();
    ~LinkedList();
    int size(){ return _size; }
    Node* head(){ return _head; }
    Node* tail(){ return _tail; }
    // pre 노드 뒤에 새로운 노드 삽입 (맨 앞에 삽입하고 싶으면 pre를 nullptr로 설정)
    void insertNode(Node* pre, Node* node);
    // node를 제거
    void deleteNode(Node* node);
    // node 노드의 위치를 pre 노드의 뒤로 이동
    void moveNode(Node* pre, Node* node);
    // key가 일치하는 node 검색 (실패 시, nullptr 반환)
    // LinkedList::findNode()는 선형 탐색 이용 (Cache::get에서는 해시 테이블 이용)
    Node* findNode(std::string key);
  };

  class HashTable{
  private:
    // index : 'A' ~ 'Z'
    LinkedList hashTable[26];

    // key의 hashcode 반환
    int hash(std::string key);

  public:
    // 테이블에 아이템 삽입
    void addItem(Node* node);
    // 테이블에서 아이템 삭제
    void removeItem(Node* node);
    // 테이블에서 아이템 검색
    Node* searchItem(std::string key);
  };

  // --------------------------------------------------
  
  LinkedList cache;
  HashTable table;

  // double 값을 문자열로 변환
  std::string dblToStr(double value);

public:
  Cache();
  ~Cache();
  // int를 cache에 추가한다
  void add(std::string key, int value);
  // double을 cache에 추가한다
  void add(std::string key, double value);
  // key에 해당하는 value를 cache에서 가져온다
  // 타입과 key가 동일한 원소가 없다면 false를 반환한다.
  bool get(std::string key, int &value);
  // key에 해당하는 value를 cache에서 가져온다.
  // 타입과 key가 동일한 원소가 없다면 false를 반환한다.
  bool get(std::string key, double &value);

  // 디버그 및 채점 용: 연결 리스트를 문자열로 표현하여 반환한다
  // 다음과 같이 표현된 문자열을 반환한다
  // [key1: value1] -> [key2: value2] -> ... -> [keyN: valueN]
  std::string toString();
};

#endif