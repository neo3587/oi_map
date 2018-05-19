# oi_map
oi_map (ordered insertion map) is a container class that emulates a std::map but using the insertion order when iterating, in addition, it provides a secondary iterator (called m_iterator) to use the predicate order.
Also includes an implementation for std::multimap, std::unordered_map and std::unordered_multimap.

# Example
```c++
  #include "oi_map.hpp"  
  #include <iostream>
  
  int main(int argc, char* argv[]) {
    
    neo::oi_map<int, char> mp;
    mp[1] = 'a';
    mp[2] = 'b';
    mp[0] = 'c';
    
    for(auto it = mp.begin(); it != mp.end(); ++it) // will print: 1,a/2,b/0,c
      std::cout << it->first << "," << it->second << "/";

    for(auto it = mp.m_begin(); it != mp.m_end(); ++it) // // will print: 0,c/1,a/2,b
      std::cout << it->first << "," << it->second << "/";

    return 0;
  }
```
