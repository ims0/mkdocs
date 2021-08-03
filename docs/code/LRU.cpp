#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

using namespace std;
template <typename KeyT, typename ValueT> class LRUCache {

public:
  typedef std::pair<KeyT, ValueT> Pair;
  typedef std::list<Pair> List;
  typedef std::unordered_map<KeyT, typename List::iterator> Map;

  explicit LRUCache(int cap) : m_capacity(cap){};
  ~LRUCache() {
    m_list.clear();
    m_map.clear();
  };
  bool put(KeyT key, ValueT value);
  bool get(KeyT key, ValueT &pValue);
  size_t size() const { return m_list.size(); };
  LRUCache(const LRUCache &) = delete;
  LRUCache &operator=(const LRUCache &) = delete;

private:
  size_t m_capacity;
  List m_list;
  Map m_map;
};

template <typename KeyT, typename ValueT>
bool LRUCache<KeyT, ValueT>::put(KeyT key, ValueT value) {
  typename Map::iterator iter = m_map.find(key);
  if (iter != m_map.end()) {
    m_list.erase(iter->second);
    m_map.erase(iter);
  }
  m_list.push_front(std::make_pair(key, value));
  m_map[key] = m_list.begin();
  if (m_list.size() > m_capacity) {
    KeyT endkey = m_list.back().first;
    m_list.pop_back();
    m_map.erase(endkey);
  }
  return true;
}
template <typename KeyT, typename ValueT>
bool LRUCache<KeyT, ValueT>::get(KeyT key, ValueT &pValue) {
  typename Map::iterator iter = m_map.find(key);
  if (iter == m_map.end()) {
    return false;
  }
  typename List::iterator listIt = iter->second;
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
