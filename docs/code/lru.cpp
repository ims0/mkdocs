#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

using namespace std;
template <typename KeyT, typename ValueT> class LRUCache {

public:
  using Pair = std::pair<KeyT, ValueT>;
  using List = std::list<Pair>;
  using Map = std::unordered_map<KeyT, typename List::iterator>;

  void put(KeyT key, ValueT value);
  bool get(KeyT key, ValueT &pValue);//返回值bool

  explicit LRUCache(int cap) : m_capacity(cap){};
  ~LRUCache() {
    m_list.clear();
    m_map.clear();
  };
  LRUCache(const LRUCache &) = delete;
  LRUCache &operator=(const LRUCache &) = delete;

private:
  size_t m_capacity;
  List m_list;//链表存的是key,val
  Map m_map;//key,list::iter
};

template <typename KeyT, typename ValueT>
void LRUCache<KeyT, ValueT>::put(KeyT key, ValueT value) {
  auto iter = m_map.find(key);
  if (iter != m_map.end()) {
    m_list.erase(iter->second);
  }
  m_list.push_front(make_pair(key, value));
  m_map[key] = m_list.begin();
  if (m_list.size() > m_capacity) {
    m_map.erase(m_list.back().first);//map也要删除
    m_list.pop_back(); //列表最后一个元素删除
  }
}
template <typename KeyT, typename ValueT>
bool LRUCache<KeyT, ValueT>::get(KeyT key, ValueT &pValue) {
  auto iter = m_map.find(key);
  if (iter == m_map.end()) {
    return false;
  }
  auto listIt = iter->second;
  m_list.push_front(*listIt);
  m_list.erase(listIt);
  m_map[key] = m_list.begin();
  pValue = listIt->second;
  return true;
}

int main() {
  LRUCache<int, string> lrucache(3);
  lrucache.put(1, "A");
  lrucache.put(2, "B");
  lrucache.put(3, "C");
  std::string value;
  bool ret = lrucache.get(1, value);
  cout << "value=" << value << ",ret=" << ret << endl;
  lrucache.put(4, "D");

  value = "";
  ret = lrucache.get(2, value);
  cout << "value=" << value << ",ret=" << ret << endl;
  return 0;
}

