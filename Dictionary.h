/**
 * @file Dictionary.h - The Dictionary ADT
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC2430, Spring 2018"
 */

#pragma once

/**
 * @class Dictionary ADT - collection of KeyType, ValueType pairs, indexable by key
 *
 * A dictionary is a set whose elements has an associated payload.
 * An element is either in the dictionary or not, solely as determined
 * by the equality operator (operator==). There is no
 * concept in this class of multiple equivalent elements.
 *
 * Implementations of this ADT are also expected to support
 * a const_iterator and methods begin() and end(). Order of
 * iteration can be arbitrary.
 *
 * @tparam KeyType     type of elements (keys) of the dictionary
 *                     must support equality operator
 * @tparam ValueType   type of associated data for each key in the dictionary
 *                     must support zero-argument construction
 */
template <typename KeyType, typename ValueType>
class Dictionary {
public:
    /**
     * Determine if the given key is currently in this dictionary
     *
     * @param key  possible element of this dictionary
     * @return     true if key is an element, false otherwise
     */
    virtual bool has(const KeyType& key) const = 0;

    /**
     * Get or create a value associated with this key.
     *
     * The returned reference is modifiable.
     *
     * If key was not already in the dictionary, this call
     * automatically creates a default-constructed ValueType
     * and associates it with key.
     * @param key   an element of this dictionary
     * @return      the value corresponding to this key
     * @post        if prior has(key) was false, then get(key) == ValueType()
     */
    virtual ValueType& get(const KeyType& key) = 0;

    /**
     * Fetch the current value associated with this key.
     *
     * The returned reference is const.
     *
     * If the key was not already in the dictionary, this call
     * throws an exception.
     * @param key   an element of this dictionary
     * @return      the value corresponding to this key
     * @throws      invalid_argument if key not a member
     * @pre         has(key) is true
     */
    virtual const ValueType& get(const KeyType& key) const = 0;

    /**
     * Add the given key, value pair into this dictionary
     *
     * May or may not already be an element of this dictionary.
     * If already in the dictionary, the prior value for key
     * is discarded.
     * @param key    an element of this dictionary
     * @param value  the data to associate with key
     * @post         has(key) is true, get(key) == value
     */
    virtual void add(const KeyType& key, const ValueType& value) = 0;

    /**
     * Remove the given key from this dictionary
     *
     * It is irrelevant how many times this key has been added to the dictionary
     * or even if it is currently an element or not.
     * @param key  an element (possibly) of this dictionary
     * @post       has(key) is false
     */
    virtual void remove(const KeyType& key) = 0;

    virtual ~Dictionary() = default;  // make the destructors virtual
};
