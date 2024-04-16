#include "cached_runner.h"
#include "task_runner.h"

// TODO: 필요한 함수 구현

CachedRunner::CachedRunner(Cache &cache) : cache(cache){
    _hits=0;
    _misses=0;
}

// 파일의 숫자를 전부 곱하여 반환한다
double CachedRunner::multiply(std::string filename){
    // 캐시에 task에 대한 결과값이 존재하는지 확인
    double result;
    if(cache.get(filename, result)){
        // 캐시에 값이 있으면
        _hits++;
    }
    else{
        // 캐시에 값이 없으면
        _misses++;

        // taskRunner 실행해서 결과값 받아오기
        result=TaskRunner::multiply(filename);

        // 캐시에 결과값 저장하기
        cache.add(filename, result);
    }
    return result;
}

// 파일에서 가장 긴 팰린드롬을 찾아 인덱스를 반환한다
int CachedRunner::palindrome(std::string filename){
    // 캐시에 task에 대한 결과값이 존재하는지 확인
    int result;
    if(cache.get(filename, result)){
        // 캐시에 값이 있으면
        _hits++;
    }
    else{
        // 캐시에 값이 없으면
        _misses++;

        // taskRunner 실행해서 결과값 받아오기
        result=TaskRunner::palindrome(filename);

        // 캐시에 결과값 저장하기
        cache.add(filename, result);
    }
    return result;
}

// 캐시에 히트한 횟수를 반환한다
int CachedRunner::hits(){
    return _hits;
}

// 캐시에 미스한 횟수를 반환한다
int CachedRunner::misses(){
    return _misses;
}