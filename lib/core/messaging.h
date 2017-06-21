#ifndef MESSAGING_H
#define MESSAGING_H

#include "core/any.hpp"
#include <map>
#include <typeinfo>
#include <functional>
#include <string>
#include <vector>

typedef std::size_t StringHash;
typedef std::hash<std::string> StringHasher;
typedef linb::any Any;
typedef std::map<StringHash, Any> AnyMap;
typedef std::vector<Any> AnyVector;

#endif // MESSAGING_H
