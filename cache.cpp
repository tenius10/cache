#include "cache.h"

// TODO: 필요한 함수 구현

Cache::Cache(){
   // 특별히 할 일이 없다
}

Cache::~Cache(){
    // LinkedList의 동적 할당 해제는 LinkedList의 소멸자에서 수행한다
}


// int를 cache에 추가한다
void Cache::add(std::string key, int value){    
    // 1. 캐시에 데이터 추가
    
    // 캐시 용량 부족하면 head 내보내기
    if(cache.size()>=CACHE_SIZE){
        // 해시 테이블에서 제거하기
        table.removeItem(cache.head());
        // 캐시에서 쫓아내기
        cache.deleteNode(cache.head());
    }

    // Node 생성
    Entry* entry=new Entry;
    entry->key=key;
    entry->type=TYPE_INT;
    entry->value=new int;
    *((int*)entry->value)=value;

    Cache::Node* node=new Cache::Node;
    node->entry=entry;

    // tail 뒤에 새로운 노드 삽입
    cache.insertNode(cache.tail(), node);

    // 2. 해시 테이블에 데이터 추가
    table.addItem(node);
}

// double을 cache에 추가한다
void Cache::add(std::string key, double value){
    // 1. 캐시에 데이터 추가
    
    // 캐시 용량 부족하면 head 내보내기
    if(cache.size()>=CACHE_SIZE){
        // 해시 테이블에서 제거하기
        table.removeItem(cache.head());
        // 캐시에서 쫓아내기
        cache.deleteNode(cache.head());
    }

    // Node 생성
    Entry* entry=new Entry;
    entry->key=key;
    entry->type=TYPE_DOUBLE;
    entry->value=new double;
    *((double*)entry->value)=value;

    Cache::Node* node=new Cache::Node;
    node->entry=entry;

    // tail 뒤에 새로운 노드 삽입
    cache.insertNode(cache.tail(), node);

    // 2. 해시 테이블에 데이터 추가
    table.addItem(node);
}


// key에 해당하는 value를 cache에서 가져온다
// 타입과 key가 동일한 원소가 없다면 false를 반환한다.
bool Cache::get(std::string key, int &value){
    // node 찾기
    Cache::Node* node=table.searchItem(key);
    if(node==nullptr) return false;
    else value=*((int*)node->entry->value);

    // node를 tail 뒤로 이동
    cache.moveNode(cache.tail(), node);

    return true;
}

// key에 해당하는 value를 cache에서 가져온다.
// 타입과 key가 동일한 원소가 없다면 false를 반환한다.
bool Cache::get(std::string key, double &value){
    // node 찾기
    Cache::Node* node=table.searchItem(key);
    if(node==nullptr) return false;
    else value=*((double*)node->entry->value);

    // node를 tail 뒤로 이동
    cache.moveNode(cache.tail(), node);

    return true;
}

// 디버그 및 채점 용: 연결 리스트를 문자열로 표현하여 반환한다
// 다음과 같이 표현된 문자열을 반환한다
// [key1: value1] -> [key2: value2] -> ... -> [keyN: valueN]
std::string Cache::toString(){
    std::string result="";
    Cache::Node* cur=cache.tail();
    while(cur!=nullptr){
        result+="["+cur->entry->key+": ";
        // INT냐 DOUBLE이냐에 따라 다르게 출력
        switch(cur->entry->type){
            case TYPE_INT:
                result+=std::to_string(*((int*)cur->entry->value));
                break;
            case TYPE_DOUBLE:
                result+=dblToStr(*((double*)cur->entry->value));
                break;
            default:
                break;
        }
        result+="]";
        if(cur->prev!=nullptr) result+=" -> ";
        cur=cur->prev;
    }
    result+="\n";
    return result;
}

// double 값을 문자열로 변환
std::string Cache::dblToStr(double value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}


// --------------------------------------------------
// 과제 채점 편의를 위해 파일을 분리하면 안 된다는 규칙 때문에 
// HashTable.cpp를 따로 작성하지 않고 아래에 작성합니다.

// key의 hashcode 반환
int Cache::HashTable::hash(std::string key) {
    int sum=0;
    for(int i=0;i<key.length();i++){
        sum+=key[i];
    }
    return sum % TABLE_SIZE;  // 0 ~ (TABLE_SIZE-1)
}

// 테이블에 아이템 삽입
void Cache::HashTable::addItem(Cache::Node* node){
    int hashCode=hash(node->entry->key);
    LinkedList& list=hashTable[hashCode];

    // Node 생성
    Entry* entry=new Entry;
    entry->key=node->entry->key;
    entry->type=TYPE_NODE_POINTER;
    entry->value=new (Node*);
    *((Node**)entry->value)=node;

    Cache::Node* tableNode=new Cache::Node;
    tableNode->entry=entry;

    // tail 뒤에 새로운 노드 삽입
    list.insertNode(list.tail(), tableNode);
}

// 테이블에서 아이템 삭제
void Cache::HashTable::removeItem(Cache::Node* node){
    int hashCode=hash(node->entry->key);
    LinkedList& list=hashTable[hashCode];

    // 매개변수로 받은 node의 key를 entry->key로 하는 해시 테이블 상의 리스트 노드를 넣어줘야 함.
    Node* oldNode=list.findNode(node->entry->key);
    list.deleteNode(oldNode);
}

// 테이블에서 아이템 검색
Cache::Node* Cache::HashTable::searchItem(std::string key){
    int hashCode=hash(key);
    LinkedList& list=hashTable[hashCode];
    Cache::Node* node=list.findNode(key);

    // 해시 테이블에 있는 LinkedList는 value가 가리키는 값이 Node* 타입이므로
    // 반환할 때는 node가 아니라 node->entry->value를 반환해야 한다. 
    if(node!=nullptr) return *((Node**)node->entry->value);
    
    return nullptr;
}


// --------------------------------------------------
// 과제 채점 편의를 위해 파일을 분리하면 안 된다는 규칙 때문에 
// LinkedList.cpp를 따로 작성하지 않고 아래에 작성합니다.

Cache::LinkedList::LinkedList(){
    // 연결 리스트 초기화
    _head=nullptr;
    _tail=nullptr;
    _size=0;
}

Cache::LinkedList::~LinkedList(){
    // 연결 리스트 할당 해제
    Cache::Node* cur=_head;
    Cache::Node* next;
    while(cur!=nullptr){
        next=cur->next;
        deleteNode(cur);
        cur=next;
    }
    _head=nullptr;
    _tail=nullptr;
}

// pre 노드 뒤에 새로운 노드 삽입 (맨 앞에 삽입하고 싶으면 pre를 nullptr로 설정)
void Cache::LinkedList::insertNode(Cache::Node* pre, Cache::Node* node){  
    if(_size==0){
        // 빈 리스트이면 head, tail 설정
        node->prev=nullptr;
        node->next=nullptr;
        _head=_tail=node;
    }
    else{
        if(pre==nullptr){
            // 맨 앞에 삽입하려는 경우
            node->prev=nullptr;
            node->next=_head;
            _head->prev=node;
            _head=node;
        }
        else{
            Cache::Node* next=pre->next;
            node->prev=pre;
            node->next=next;
            if(pre==_tail) _tail=node;
            if(pre!=nullptr) pre->next=node;
            if(next!=nullptr) next->prev=node;
        }
    }
    _size++;
}

// node를 제거
void Cache::LinkedList::deleteNode(Cache::Node* node){
    if(node==nullptr) return;

    // 연결 끊기
    Cache::Node* pre=node->prev;
    Cache::Node* next=node->next;
    
    if(pre!=nullptr) pre->next=node->next;
    if(next!=nullptr) next->prev=node->prev;

    if(node==_head) _head=next;
    if(node==_tail) _tail=pre;
    
    _size--;

    // 메모리 할당 해제
    switch(node->entry->type){
        case TYPE_INT:
            delete (int*)node->entry->value;
            break;
        case TYPE_DOUBLE:
            delete (double*)node->entry->value;
            break;
        case TYPE_NODE_POINTER:
            delete (Cache::Node**)node->entry->value;
            break;
    }
    delete node->entry;
    delete node;
}

// node의 위치를 pre 노드의 뒤로 이동
void Cache::LinkedList::moveNode(Cache::Node* pre, Cache::Node* node){
    // 위치를 변경할 필요가 없는 경우
    if(pre==node) return;

    // node의 연결 끊기
    Cache::Node* prev=node->prev;
    Cache::Node* next=node->next;

    if(prev!=nullptr) prev->next=node->next;
    if(next!=nullptr) next->prev=node->prev;

    if(node==_head) _head=next;
    if(node==_tail) _tail=prev;

    _size--;

    // node를 pre 뒤에 삽입하기
    insertNode(pre, node);
}

// key가 일치하는 node 검색 (실패 시, nullptr 반환)
// LinkedList::findNode()는 선형 탐색 이용 (Cache::get에서는 해시 테이블 이용)
Cache::Node* Cache::LinkedList::findNode(std::string key){
    Cache::Node* cur=_head;
    while(cur!=nullptr){
        if(cur->entry->key==key) return cur;
        cur=cur->next;
    }
    return nullptr;
}
