
#pragma once

#ifndef __NEO_OI_MAP_HPP__
#define __NEO_OI_MAP_HPP__

/*
	Header name: oi_map.hpp (ordered insertion map)
	Author: neo3587

	http://www.cplusplus.com/reference/map/map/
	

	Notes:
		- Requires C++11 or higher
		- m_iterators follows the predicate order instead of the insertion order (just like a normal std::map)
		- emplace_hint is just there for compatibility, it won't speed up the insertions

	TODO:
		- oi_multimap
		- oi_unordered_map
		- oi_unordered_multimap

*/



#include <map>
#include <list>
#include <initializer_list>



namespace neo {

	template<class Key, class Value, class Predicate = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Value>>>
	class oi_map {

		private:

			using _list_t = std::list<std::pair<const Key, Value>>;
			using _map_t  = std::map<const Key, typename _list_t::iterator, Predicate, std::allocator<std::pair<const Key, typename _list_t::iterator>>>;

			_list_t _list;
			_map_t	_map;

			template<class MappedIter, class MapIterator>
			class Iterator {

				public:

					using iterator_category = std::bidirectional_iterator_tag;
					using value_type		= typename MappedIter::value_type;
					using difference_type	= typename MappedIter::difference_type;
					using pointer			= typename MappedIter::pointer;
					using reference			= typename MappedIter::reference;

					Iterator() {}
					Iterator(const Iterator&) = default;
					Iterator(Iterator&&) = default;

					bool operator==(const Iterator& other) const {
						return _iter == other._iter;
					}
					bool operator!=(const Iterator& other) const {
						return !(*this == other);
					}

					Iterator& operator++() {
						++_iter;
						return *this;
					}
					Iterator operator++(int) {
						Iterator tmp(*this);
						++(*this);
						return tmp;
					}

					Iterator& operator--() {
						--_iter;
						return *this;
					}
					Iterator operator--(int) {
						Iterator tmp(*this);
						--(*this);
						return tmp;
					}

					reference operator*() {
						return *(_iter->second);
					}
					const reference operator*() const {
						return const_cast<Iterator&>(*this).operator*();
					}

					pointer operator->() {
						return _iter->second.operator->();
					}
					const pointer operator->() const {
						return const_cast<Iterator&>(*this).operator->();
					}

				protected:

					Iterator(const MapIterator& iter) : _iter(iter) {}

					MapIterator _iter;

			};

		public:

			// Member types:

			class m_const_iterator; // forward declaration

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

			using iterator					= typename _list_t::iterator;
			using const_iterator			= typename _list_t::const_iterator;
			using reverse_iterator			= typename _list_t::reverse_iterator;
			using const_reverse_iterator	= typename _list_t::const_reverse_iterator;

			class m_iterator : public Iterator<typename _list_t::iterator, typename _map_t::iterator> {
				public:
					using Iterator<typename _list_t::iterator, typename _map_t::iterator>::Iterator;
					friend m_const_iterator;
					friend oi_map;
			};
			class m_const_iterator : public Iterator<typename _list_t::const_iterator, typename _map_t::const_iterator> {
				public:
					using Iterator<typename _list_t::const_iterator, typename _map_t::const_iterator>::Iterator;
					m_const_iterator(const m_iterator& other) {
						this->_iter = other._iter;
					}
					friend oi_map;
			};
			using m_reverse_iterator		= std::reverse_iterator<m_iterator>;
			using m_const_reverse_iterator	= std::reverse_iterator<m_const_iterator>;

			// Constructors:

			oi_map() {}
			explicit oi_map(const allocator_type& alloc) {}

			oi_map(const oi_map&) = default;
			oi_map(const oi_map& other, const allocator_type& alloc) {
				insert(other.begin(), other.end());
			}

			oi_map(oi_map&&) = default;
			oi_map(oi_map&& other, const allocator_type& alloc) {
				swap(other);
			}

			oi_map& operator=(const oi_map&) = default;
			oi_map& operator=(oi_map&&) = default;

			// Iterators:

			iterator begin() {
				return _list.begin();
			}
			const_iterator begin() const {
				return _list.begin();
			}
			const_iterator cbegin() const {
				return _list.cbegin();
			}
			iterator end() {
				return _list.end();
			}
			const_iterator end() const {
				return _list.end();
			}
			const_iterator cend() const {
				return _list.cend();
			}

			reverse_iterator rbegin() {
				return _list.rbegin();
			}
			const_reverse_iterator rbegin() const {
				return _list.rbegin();
			}
			const_reverse_iterator crbegin() const {
				return _list.crbegin();
			}
			reverse_iterator rend() {
				return _list.rend();
			}
			const_reverse_iterator rend() const {
				return _list.rend();
			}
			const_reverse_iterator crend() const {
				return _list.crend();
			}

			m_iterator m_begin() {
				return _map.begin();
			}
			m_const_iterator m_begin() const {
				return _map.begin();
			}
			m_const_iterator m_cbegin() const {
				return _map.cbegin();
			}
			m_iterator m_end() {
				return _map.end();
			}
			m_const_iterator m_end() const {
				return _map.end();
			}
			m_const_iterator m_cend() const {
				return _map.end();
			}

			m_reverse_iterator m_rbegin() {
				return _map.rbegin();
			}
			m_const_reverse_iterator m_rbegin() const {
				return _map.rbegin();
			}
			m_const_reverse_iterator m_crbegin() const {
				return _map.crbegin();
			}
			m_reverse_iterator m_rend() {
				return _map.rend();
			}
			m_const_reverse_iterator m_rend() const {
				return _map.rend();
			}
			m_const_reverse_iterator m_crend() const {
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

			// Element access:

			mapped_type& operator[](const key_type& key) {
				return (*((insert(std::make_pair(key, mapped_type()))).first)).second;
			}
			mapped_type& operator[](key_type&& key) {
				return (*((insert(std::make_pair(std::forward<key_type>(key), mapped_type()))).first)).second;
			}

			mapped_type& at(const key_type& key) {
				return _map.at(key)->second;
			}
			const mapped_type& at(const key_type& key) const {
				return _map.at(key)->second;
			}

			// Modifiers:

			std::pair<iterator, bool> insert(const value_type& val) {
				typename _map_t::iterator it_key = _map.find(val.first);
				if(it_key != _map.end()) {
					return std::pair<iterator, bool>(it_key->second, false);
				}
				_list.push_back(val);
				iterator it_list = --_list.end();
				_map.emplace_hint(it_key, it_list->first, it_list);
				return std::pair<iterator, bool>(it_list, true);
			}
			template<class P>
			std::pair<iterator, bool> insert(P&& val) {
				typename _map_t::iterator it_key = _map.find(val.first);
				if(it_key != _map.end()) {
					return std::pair<iterator, bool>(it_key->second, false);
				}
				_list.push_back(std::forward<P>(val));
				iterator it_list = --_list.end();
				_map.emplace_hint(it_key, it_list->first, it_list);
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
				_map.erase(pos->first);
				return _list.erase(pos._get());
			}
			size_type erase(const key_type& key) {
				typename _map_t::const_iterator it = _map.find(key);
				if(it != _map.end()) {
					_list.erase(it->second);
					_map.erase(it);
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

			void swap(oi_map& other) {
				_list.swap(other._list);
				_map.swap(other._map);
			}

			void clear() noexcept {
				_list.clear();
				_map.clear();
			}

			template<class... Args>
			std::pair<iterator, bool> emplace(Args&&... args) {
				return insert(std::pair<Args...>(std::forward<Args>(args)...));
			}
			template<class... Args>
			iterator emplace_hint(const_iterator position, Args&&... args) {
				return insert(std::pair<Args...>(std::forward<Args>(args)...)).first;
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

			iterator lower_bound(const key_type& key) {
				return _map.lower_bound(key)->second;
			}
			const_iterator lower_bound(const key_type& key) const {
				return _map.lower_bound(key)->second;
			}
			iterator upper_bound(const key_type& key) {
				return _map.upper_bound(key)->second;
			}
			const_iterator upper_bound(const key_type& key) const {
				return _map.upper_bound(key)->second;
			}

			std::pair<iterator, iterator> equal_range(const key_type& key) {
				std::pair<typename _map_t::iterator, typename _map_t::iterator> ret = _map.equal_range(key);
				return std::pair<iterator, iterator>(ret.first->second, ret.second->second);
			}
			std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
				std::pair<typename _map_t::const_iterator, typename _map_t::const_iterator> ret = _map.equal_range(key);
				return std::pair<const_iterator, const_iterator>(ret.first->second, ret.second->second);
			}

			// Allocator:

			allocator_type get_allocator() const noexcept {
				return allocator_type();
			}

	};

}



#endif

