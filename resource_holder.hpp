#ifndef __RESOURCE_HOLDER_HPP__
#define __RESOURCE_HOLDER_HPP__

#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

template <typename Resource, typename ID>
class ResourceHolder {
    std::map<ID, std::unique_ptr<Resource>> _resourceMap;

public:
    ResourceHolder() = default;
    ResourceHolder(const ResourceHolder<Resource, ID> &) = delete;
    
    void load(ID id, const std::string &filename)
    {
        std::unique_ptr<Resource> resource(new Resource());
        if (!resource->loadFromFile(filename))
            throw std::runtime_error("ResourceHolder: failed to load " + filename);

        insertResource(id, std::move(resource));
    }

    template <typename Parameter>
    void load(ID id, const std::string &filename, const Parameter &secondParam)
    {
        std::unique_ptr<Resource> resource(new Resource());
        if (!resource->loadFromFile(filename, secondParam))
            throw std::runtime_error("ResourceHolder: failed to load " + filename);

        insertResource(id, std::move(resource));
    }

    Resource &get(ID id)
    {
        auto found = _resourceMap.find(id);
#ifndef NDEBUG
        assert(found != _resourceMap.end());
#endif

        return *found->second;
    };
    const Resource &get(ID id) const
    {
        auto found = _resourceMap.find(id);
#ifndef NDEBUG
        assert(found != _resourceMap.end());
#endif

        return *found->second;
    }

private:
    void insertResource(ID id, std::unique_ptr<Resource> resource)
    {
        const auto &inserted = _resourceMap.insert({id, std::move(resource)});
#ifndef NDEBUG
        assert(inserted.second);
#endif
    }
};

#endif