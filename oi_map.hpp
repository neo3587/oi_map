
#pragma once

#ifndef __NEO_OI_MAP_HPP__
#define __NEO_OI_MAP_HPP__

/*
	Header name: oi_map.hpp (ordered insertion map)
	Author: neo3587

	http://www.cplusplus.com/reference/map/map/
	http://www.cplusplus.com/reference/map/multimap/
	http://www.cplusplus.com/reference/unordered_map/unordered_map/
	http://www.cplusplus.com/reference/unordered_map/unordered_multimap/

	Notes:
		- Requires C++11 or higher
		- m_iterators follows the predicate order instead of the insertion order (just like a normal std::map)
		- m_iterators are convertible to iterators, but not vice versa
		- oi_multimap uses m_iterators for lower_bound(), upper_bound() and equal_range(), iterators that follows the insertion order cannot work properly with these functions
		- emplace_hint is just there for compatibility, it won't speed up the insertions

	TODO:
		- oi_unordered_map
		- oi_unordered_multimap

*/



#include <map>
#include <unordered_map>
#include <list>
#include <initializer_list>



namespace neo {

	namespace __oi_map_details {

		template<class MappedIter, class MapIterator>
		class oi_iterator {

			public:

				using iterator_category = std::bidirectional_iterator_tag;
				using value_type		= typename MappedIter::value_type;
				using difference_type	= typename MappedIter::difference_type;
				using pointer			= typename MappedIter::pointer;
				using reference			= typename MappedIter::reference;

				oi_iterator() {}
				oi_iterator(const oi_iterator&) = default;
				oi_iterator(oi_iterator&&) = default;
				oi_iterator& operator=(const oi_iterator&) = default;
				oi_iterator& operator=(oi_iterator&&) = default;
				oi_iterator(const MapIterator& iter) : _iter(iter) {}
				oi_iterator(MapIterator&& iter) : _iter(std::forward<MapIterator>(iter)) {}

				bool operator==(const oi_iterator& other) const {
					return _iter == other._iter;
				}
				bool operator!=(const oi_iterator& other) const {
					return !(*this == other);
				}

				oi_iterator& operator++() {
					++_iter;
					return *this;
				}
				oi_iterator operator++(int) {
					oi_iterator tmp(*this);
					++(*this);
					return tmp;
				}

				oi_iterator& operator--() {
					--_iter;
					return *this;
				}
				oi_iterator operator--(int) {
					oi_iterator tmp(*this);
					--(*this);
					return tmp;
				}

				reference operator*() {
					return _iter->second.operator*();
				}
				const reference operator*() const {
					return _iter->second.operator*();
				}

				pointer operator->() {
					return _iter->second.operator->();
				}
				const pointer operator->() const {
					return _iter->second.operator->();
				}

			protected:
		
				MapIterator _iter;

		};

		template<class Key, class Value, class Allocator, class Map>
		class oi_base {

			protected:
				
				using _list_t = std::list<std::pair<const Key, Value>>;
				using _map_t  = Map;

				_list_t _list;
				_map_t	_map;

			public:

				// Member types:

				using key_type					= Key;
				using mapped_type				= Value;
				using value_type				= std::pair<const key_type, mapped_type>;
				using size_type					= typename _map_t::size_type;
				using difference_type			= typename _map_t::difference_type;

				using allocator_type			= Allocator;

				using pointer					= typename Allocator::pointer;
				using const_pointer				= typename Allocator::const_pointer;
				using reference					= typename Allocator::reference;
				using const_reference			= typename Allocator::const_reference;

				using key_compare				= typename _map_t::key_compare;
				using value_compare				= typename _map_t::value_compare;

				class iterator;
				class const_iterator;
				class m_iterator;
				class m_const_iterator;

				class iterator : public _list_t::iterator {
					public:
						iterator() {}
						iterator(const iterator&) = default;
						iterator(iterator&&) = default;
						iterator& operator=(const iterator&) = default;
						iterator& operator=(iterator&&) = default;
						
						iterator(const m_iterator& other) : _list_t::iterator(other._iter->second) {}
						iterator(m_iterator&& other) : _list_t::iterator(std::forward<m_iterator>(other)._iter->second) {}

						iterator(const typename _list_t::iterator& other) : _list_t::iterator(other) {}
						iterator(typename _list_t::iterator&& other) : _list_t::iterator(other) {}
				};
				class const_iterator : public _list_t::const_iterator {
					public:
						const_iterator() {}
						const_iterator(const const_iterator&) = default;
						const_iterator(const_iterator&&) = default;
						const_iterator& operator=(const const_iterator&) = default;
						const_iterator& operator=(const_iterator&&) = default;

						const_iterator(const iterator& other) : _list_t::const_iterator(other) {}
						const_iterator(iterator&& other) : _list_t::const_iterator(std::forward<iterator>(other)) {}

						const_iterator(const m_iterator& other) : _list_t::const_iterator(other._iter->second) {}
						const_iterator(m_iterator&& other) : _list_t::const_iterator(std::forward<m_iterator>(other)._iter->second) {}
						const_iterator(const m_const_iterator& other) : _list_t::const_iterator(other._iter->second) {}
						const_iterator(m_const_iterator&& other) : _list_t::const_iterator(std::forward<m_const_iterator>(other)._iter->second) {}

						const_iterator(const typename _list_t::const_iterator& other) : _list_t::const_iterator(other) {}
						const_iterator(typename _list_t::const_iterator&& other) : _list_t::const_iterator(other) {}
				};
				using reverse_iterator			= std::reverse_iterator<iterator>;
				using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

				class m_iterator : public oi_iterator<iterator, typename _map_t::iterator> {
					public:
						using oi_iterator<iterator, typename _map_t::iterator>::oi_iterator;
						friend iterator;
						friend const_iterator;
						friend m_const_iterator;
						friend oi_base;
				};
				class m_const_iterator : public oi_iterator<const_iterator, typename _map_t::const_iterator> {
					public:
						using oi_iterator<const_iterator, typename _map_t::const_iterator>::oi_iterator;
						m_const_iterator(const m_iterator& other) {
							this->_iter = other._iter;
						}
						m_const_iterator(m_iterator&& other) {
							this->_iter = other._iter;
						}
						friend const_iterator;
						friend oi_base;
				};
				using m_reverse_iterator		= std::reverse_iterator<m_iterator>;
				using m_const_reverse_iterator	= std::reverse_iterator<m_const_iterator>;

				// Constructors:

				oi_base() {}
				explicit oi_base(const key_compare& comp, const allocator_type& alloc = allocator_type()) : _map(comp), _list(alloc) {}
				explicit oi_base(const allocator_type& alloc) : _list(alloc) {}

				oi_base(const oi_base&) = default;
				oi_base(const oi_base& other, const allocator_type& alloc) : _list(alloc) {
					*this = other;
				}

				oi_base(oi_base&&) = default;
				oi_base(oi_base&& other, const allocator_type& alloc) : _list(alloc) {
					*this = std::forward<oi_base>(other);
				}

				oi_base& operator=(const oi_base&) = default;
				oi_base& operator=(oi_base&&) = default;

				// Iterators:

				iterator begin() noexcept {
					return _list.begin();
				}
				const_iterator begin() const noexcept {
					return _list.begin();
				}
				const_iterator cbegin() const noexcept {
					return _list.cbegin();
				}
				iterator end() noexcept {
					return _list.end();
				}
				const_iterator end() const noexcept {
					return _list.end();
				}
				const_iterator cend() const noexcept {
					return _list.cend();
				}

				reverse_iterator rbegin() noexcept {
					return _list.rbegin();
				}
				const_reverse_iterator rbegin() const noexcept {
					return _list.rbegin();
				}
				const_reverse_iterator crbegin() const noexcept {
					return _list.crbegin();
				}
				reverse_iterator rend() noexcept {
					return _list.rend();
				}
				const_reverse_iterator rend() const noexcept {
					return _list.rend();
				}
				const_reverse_iterator crend() const noexcept {
					return _list.crend();
				}

				m_iterator m_begin() noexcept {
					return _map.begin();
				}
				m_const_iterator m_begin() const noexcept {
					return _map.begin();
				}
				m_const_iterator m_cbegin() const noexcept {
					return _map.cbegin();
				}
				m_iterator m_end() noexcept {
					return _map.end();
				}
				m_const_iterator m_end() const noexcept {
					return _map.end();
				}
				m_const_iterator m_cend() const noexcept {
					return _map.end();
				}

				m_reverse_iterator m_rbegin() noexcept {
					return _map.rbegin();
				}
				m_const_reverse_iterator m_rbegin() const noexcept {
					return _map.rbegin();
				}
				m_const_reverse_iterator m_crbegin() const noexcept {
					return _map.crbegin();
				}
				m_reverse_iterator m_rend() noexcept {
					return _map.rend();
				}
				m_const_reverse_iterator m_rend() const noexcept {
					return _map.rend();
				}
				m_const_reverse_iterator m_crend() const noexcept {
					return _map.rend();
				}

				// Capacity:

				bool empty() const noexcept {
					return _map.empty();
				}
				size_type size() const noexcept {
					return _map.size();
				}
				size_type max_size() const noexcept {
					return _map.max_size();
				}

				// Modifiers:
				
				void swap(oi_base& other) {
					_list.swap(other._list);
					_map.swap(other._map);
				}

				void clear() noexcept {
					_list.clear();
					_map.clear();
				}

				// Observers:

				key_compare key_comp() const {
					return key_compare();
				}
				value_compare value_comp() const {
					return value_compare(key_comp());
				}

				// Operations:

				iterator find(const key_type& key) {
					typename _map_t::iterator it_map = _map.find(key);
					return it_map != _map.end() ? it_map->second : end();
				}
				const_iterator find(const key_type& key) const {
					typename _map_t::const_iterator it_map = _map.find(key);
					return it_map != _map.end() ? it_map->second : end();
				}

				size_type count(const key_type& key) const {
					return _map.count(key);
				}

				// Allocator:

				allocator_type get_allocator() const noexcept {
					return allocator_type();
				}
		};

		template<class Key, class Value, class Allocator, class Map>
		class oi_single : public virtual oi_base<Key, Value, Allocator, Map> {

			protected:

				using _list_t			= typename oi_single::_list_t;
				using _map_t			= typename oi_single::_map_t;

			public:

				// Member Types

				using key_type			= typename oi_single::key_type;
				using mapped_type		= typename oi_single::mapped_type;
				using value_type		= typename oi_single::value_type;
				using size_type			= typename oi_single::size_type;

				using iterator			= typename oi_single::iterator;
				using const_iterator	= typename oi_single::const_iterator;

				using key_compare		= typename oi_single::key_compare;
				using allocator_type	= typename oi_single::allocator_type;

				// Constructor:

				using oi_base<Key, Value, Allocator, _map_t>::oi_base;
				oi_single() {} // GCC complains w/o this
				template<class InputIterator>
				oi_single(InputIterator left, InputIterator right, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : oi_base<Key, Value, Allocator, _map_t>(comp, alloc) {
					insert(left, right);
				}
				oi_single(std::initializer_list<value_type> il, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : oi_base<Key, Value, Allocator, _map_t>(comp, alloc) {
					insert(il);
				}

				// Element access:

				mapped_type& operator[](const key_type& key) {
					return (*((this->insert(std::make_pair(key, mapped_type()))).first)).second;
				}
				mapped_type& operator[](key_type&& key) {
					return (*((this->insert(std::make_pair(std::forward<key_type>(key), mapped_type()))).first)).second;
				}

				mapped_type& at(const key_type& key) {
					return this->_map.at(key)->second;
				}
				const mapped_type& at(const key_type& key) const {
					return this->_map.at(key)->second;
				}

				// Modifiers:

				std::pair<iterator, bool> insert(const value_type& val) {
					typename _map_t::iterator it_key = this->_map.find(val.first);
					if(it_key != this->_map.end()) {
						return std::pair<iterator, bool>(it_key->second, false);
					}
					this->_list.push_back(val);
					iterator it_list = --this->_list.end();
					this->_map.emplace_hint(it_key, it_list->first, it_list);
					return std::pair<iterator, bool>(it_list, true);
				}
				template<class P>
				std::pair<iterator, bool> insert(P&& val) {
					typename _map_t::iterator it_key = this->_map.find(val.first);
					if(it_key != this->_map.end()) {
						return std::pair<iterator, bool>(it_key->second, false);
					}
					this->_list.push_back(std::forward<P>(val));
					iterator it_list = --this->_list.end();
					this->_map.emplace_hint(it_key, it_list->first, it_list);
					return std::pair<iterator, bool>(it_list, true);
				}
				iterator insert(const_iterator hint, const value_type& val) {
					return insert(val).first;
				}
				template<class P>
				iterator insert(const_iterator hint, P&& val) {
					return insert(std::forward<P>(val)).first;
				}
				template<class InputIterator>
				void insert(InputIterator left, InputIterator right) {
					for(; left != right; ++left)
						insert(*left);
				}
				void insert(std::initializer_list<value_type> init_list) {
					insert(init_list.begin(), init_list.end());
				}

				iterator erase(const_iterator pos) {
					this->_map.erase(pos->first);
					return this->_list.erase(pos);
				}
				size_type erase(const key_type& key) {
					typename _map_t::const_iterator it = this->_map.find(key);
					if(it != this->_map.end()) {
						this->_list.erase(it->second);
						this->_map.erase(it);
						return 1;
					}
					return 0;
				}
				iterator erase(const_iterator left, const_iterator right) {
					iterator it;
					while(left != right)
						it = erase(left++);
					return it;
				}

				template<class... Args>
				std::pair<iterator, bool> emplace(Args&&... args) {
					return insert(std::pair<Args...>(std::forward<Args>(args)...));
				}
				template<class... Args>
				iterator emplace_hint(const_iterator hint, Args&&... args) {
					return insert(std::pair<Args...>(std::forward<Args>(args)...)).first;
				}

				// Operations:

				iterator lower_bound(const key_type& key) {
					return this->_map.lower_bound(key)->second;
				}
				const_iterator lower_bound(const key_type& key) const {
					return this->_map.lower_bound(key)->second;
				}
				iterator upper_bound(const key_type& key) {
					return this->_map.upper_bound(key)->second;
				}
				const_iterator upper_bound(const key_type& key) const {
					return this->_map.upper_bound(key)->second;
				}

				std::pair<iterator, iterator> equal_range(const key_type& key) {
					std::pair<typename _map_t::iterator, typename _map_t::iterator> ret = this->_map.equal_range(key);
					return std::pair<iterator, iterator>(ret.first->second, ret.second->second);
				}
				std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
					std::pair<typename _map_t::const_iterator, typename _map_t::const_iterator> ret = this->_map.equal_range(key);
					return std::pair<const_iterator, const_iterator>(ret.first->second, ret.second->second);
				}

		};

		template<class Key, class Value, class Allocator, class Map>
		class oi_multi : public virtual oi_base<Key, Value, Allocator, Map> {

			protected:

				using _list_t			= typename oi_multi::_list_t;
				using _map_t			= typename oi_multi::_map_t;

			public:

				// Member Types

				using key_type			= typename oi_multi::key_type;
				using mapped_type		= typename oi_multi::mapped_type;
				using value_type		= typename oi_multi::value_type;
				using size_type			= typename oi_multi::size_type;

				using iterator			= typename oi_multi::iterator;
				using const_iterator	= typename oi_multi::const_iterator;
				using m_iterator		= typename oi_multi::m_iterator;
				using m_const_iterator	= typename oi_multi::m_const_iterator;

				using key_compare		= typename oi_multi::key_compare;
				using allocator_type	= typename oi_multi::allocator_type;

				// Constructor:

				using oi_base<Key, Value, Allocator, _map_t>::oi_base;
				oi_multi() {} // GCC complains w/o this
				template<class InputIterator>
				oi_multi(InputIterator left, InputIterator right, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : oi_base<Key, Value, Allocator, _map_t>(comp, alloc) {
					insert(left, right);
				}
				oi_multi(std::initializer_list<value_type> il, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : oi_base<Key, Value, Allocator, _map_t>(comp, alloc) {
					insert(il);
				}

				// Modifiers:

				iterator insert(const value_type& val) {
					this->_list.push_back(val);
					iterator it_list = --this->_list.end();
					this->_map.emplace(it_list->first, it_list);
					return it_list;
				}
				template<class P>
				iterator insert(P&& val) {
					this->_list.push_back(std::forward<P>(val));
					iterator it_list = --this->_list.end();
					this->_map.emplace(it_list->first, it_list);
					return it_list;
				}
				iterator insert(const_iterator hint, const value_type& val) {
					return insert(val);
				}
				template<class P>
				iterator insert(const_iterator hint, P&& val) {
					return insert(std::forward<P>(val));
				}
				template<class InputIterator>
				void insert(InputIterator left, InputIterator right) {
					for(; left != right; ++left)
						insert(*left);
				}
				void insert(std::initializer_list<value_type> init_list) {
					insert(init_list.begin(), init_list.end());
				}

				iterator erase(const_iterator pos) {
					std::pair<typename _map_t::iterator, typename _map_t::iterator> eq = this->_map.equal_range(pos->first);
					for(typename _map_t::iterator it = eq.first; it != eq.second; ++it) { // cannot use erase(pos->first) like oi_map since it would delete all the keys
						if(it->second == pos) {
							this->_map.erase(it);
							break;
						}
					}
					return this->_list.erase(pos);
				}
				size_type erase(const key_type& key) {
					std::pair<typename _map_t::const_iterator, typename _map_t::const_iterator> range = this->_map.equal_range(key);
					size_type count = 0;
					for(typename _map_t::const_iterator it = range.first; it != range.second; ++it) {
						this->_list.erase(it->second);
						++count;
					}
					this->_map.erase(range.first, range.second);
					return count;
				}
				iterator erase(const_iterator left, const_iterator right) {
					iterator it;
					while(left != right)
						it = erase(left++);
					return it;
				}

				template<class... Args>
				iterator emplace(Args&&... args) {
					return insert(std::pair<Args...>(std::forward<Args>(args)...));
				}
				template<class... Args>
				iterator emplace_hint(const_iterator hint, Args&&... args) {
					return insert(std::pair<Args...>(std::forward<Args>(args)...));
				}

				// Operations:

				m_iterator lower_bound(const key_type& key) {
					return this->_map.lower_bound(key);
				}
				m_const_iterator lower_bound(const key_type& key) const {
					return this->_map.lower_bound(key);
				}
				m_iterator upper_bound(const key_type& key) {
					return this->_map.upper_bound(key);
				}
				m_const_iterator upper_bound(const key_type& key) const {
					return this->_map.upper_bound(key);
				}

				std::pair<m_iterator, m_iterator> equal_range(const key_type& key) {
					std::pair<typename _map_t::iterator, typename _map_t::iterator> ret = this->_map.equal_range(key);
					return std::pair<m_iterator, m_iterator>(ret.first, ret.second);
				}
				std::pair<m_const_iterator, m_const_iterator> equal_range(const key_type& key) const {
					std::pair<typename _map_t::const_iterator, typename _map_t::const_iterator> ret = this->_map.equal_range(key);
					return std::pair<m_const_iterator, m_const_iterator>(ret.first, ret.second);
				}

		};

		template<class Key, class Value, class Allocator, class Map>
		class oi_ordered : public virtual oi_base<Key, Value, Allocator, Map> {

		};

		template<class Key, class Value, class Allocator, class Map>
		class oi_unordered : public virtual oi_base<Key, Value, Allocator, Map> {

		};

	}

	template<class Key, class Value, class Predicate = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Value>>>
	class oi_map : public __oi_map_details::oi_ordered<Key, Value, Allocator, std::map<Key, typename std::list<std::pair<const Key, Value>>::iterator, Predicate>>,
				   public __oi_map_details::oi_single<Key, Value, Allocator, std::map<Key, typename std::list<std::pair<const Key, Value>>::iterator, Predicate>> {
		public:
			using __oi_map_details::oi_ordered<Key, Value, Allocator, std::map<Key, typename std::list<std::pair<const Key, Value>>::iterator, Predicate>>::oi_ordered;
			using __oi_map_details::oi_single<Key, Value, Allocator, std::map<Key, typename std::list<std::pair<const Key, Value>>::iterator, Predicate>>::oi_single;
	};

	template<class Key, class Value, class Predicate = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Value>>>
	class oi_multimap : public __oi_map_details::oi_ordered<Key, Value, Allocator, std::multimap<Key, typename std::list<std::pair<const Key, Value>>::iterator, Predicate>>,
						public __oi_map_details::oi_multi<Key, Value, Allocator, std::multimap<Key, typename std::list<std::pair<const Key, Value>>::iterator, Predicate>> {
		public:
			using __oi_map_details::oi_ordered<Key, Value, Allocator, std::multimap<Key, typename std::list<std::pair<const Key, Value>>::iterator, Predicate>>::oi_ordered;
			using __oi_map_details::oi_multi<Key, Value, Allocator, std::multimap<Key, typename std::list<std::pair<const Key, Value>>::iterator, Predicate>>::oi_multi;
	};

	/*
	template<class Key, class Value, class Hash = std::hash<Key>, class Predicate = std::equal_to<Key>, class Allocator = std::allocator<std::pair<const Key, Value>>>
	class oi_unordered_map : public __oi_map_details::oi_unordered<Key, Value, Allocator, std::unordered_map<Key, typename std::list<std::pair<const Key, Value>>::iterator, Hash, Predicate>>,
							 public __oi_map_details::oi_single<Key, Value, Allocator, std::unordered_map<Key, typename std::list<std::pair<const Key, Value>>::iterator, Hash, Predicate>> {
		public:
			using __oi_map_details::oi_unordered<Key, Value, Allocator, std::unordered_map<Key, typename std::list<std::pair<const Key, Value>>::iterator, Hash, Predicate>>::oi_unordered;
			using __oi_map_details::oi_single<Key, Value, Allocator, std::unordered_map<Key, typename std::list<std::pair<const Key, Value>>::iterator, Hash, Predicate>>::oi_single;
	};

	template<class Key, class Value, class Hash = std::hash<Key>, class Predicate = std::equal_to<Key>, class Allocator = std::allocator<std::pair<const Key, Value>>>
	class oi_unordered_multimap : public __oi_map_details::oi_unordered<Key, Value, Allocator, std::unordered_multimap<Key, typename std::list<std::pair<const Key, Value>>::iterator, Hash, Predicate>>,
								  public __oi_map_details::oi_multi<Key, Value, Allocator, std::unordered_multimap<Key, typename std::list<std::pair<const Key, Value>>::iterator, Hash, Predicate>> {
		public:
			using __oi_map_details::oi_unordered<Key, Value, Allocator, std::unordered_multimap<Key, typename std::list<std::pair<const Key, Value>>::iterator, Hash, Predicate>>::oi_unordered;
			using __oi_map_details::oi_multi<Key, Value, Allocator, std::unordered_multimap<Key, typename std::list<std::pair<const Key, Value>>::iterator, Hash, Predicate>>::oi_multi;
	};
	*/

}



#endif

