#pragma once 

#include <memory>
#include <string>
#include <list>
#include <map>

namespace age::res {
    
    class ResHandle;
    class ResCache;

    class IResourceLoader {
    public:
        virtual std::string vGetPattern() = 0;
        virtual bool vUseRawFile() = 0;
        virtual uint32_t vGetLoadedResourceSize(char* rawBuffer, uint32_t rawSize) = 0;
        virtual bool vLoadResource(char* rawBuffer, uint32_t rawSize, std::shared_ptr<ResHandle> handle) = 0;
    };

    class IResourceExtraData {
    public:
        virtual std::string vToString() = 0;
    };

    class IResourceFile {
    public:
        virtual ~IResourceFile() {}
        virtual bool vOpen() = 0;
        virtual int vGetRawResourceSize(const Resource& r) = 0;
        virtual int vGetRawResource(const Resource& r, char* buffer) = 0;
        virtual int vGetNumResources() const = 0;
        virtual std::string vGetResourceName(int num) const = 0;
    };

    class Resource {
    public:
        std::string m_name;
        Resource(const std::string &name);
    };

    class ResHandle {
        friend class ResCache;

    protected:
        Resource m_resource;
        char* m_buffer;
        uint32_t m_size;
        std::shared_ptr<IResourceExtraData> m_extra;
        ResCache* m_pResCache;

    public:
        ResHandle(Resource& resource, char* buffer, uint32_t size, ResCache* pResCache);

        virtual ~ResHandle();

        const std::string getName() { return m_resource.m_name; }
        uint32_t size() const { return m_size; }
        char* buffer() const { return m_buffer; }
        char* writableBuffer() { return m_buffer; }

        std::shared_ptr<IResourceExtraData> getExtra() { return m_extra; }
        void setExtra(std::shared_ptr<IResourceExtraData> extra) { m_extra = extra; }
    };

    typedef std::list<std::shared_ptr<ResHandle>> ResHandleList;
    typedef std::map<std::string, std::shared_ptr<ResHandle>> ResHandleMap;
    typedef std::list<std::shared_ptr<IResourceLoader>> ResourceLoaders;

    class ResCache {
        friend class ResHandle;

        ResHandleList m_resHandleList;
        ResHandleMap m_resources;
        ResourceLoaders m_resourceLoaders;

        IResourceFile *m_file;

        uint32_t m_cacheSize;
        uint32_t m_allocated;

    protected:
        bool makeRoom(uint32_t size);
        char* allocate(uint32_t size);
        void free(std::shared_ptr<ResHandle> resHandle);

        std::shared_ptr<ResHandle> load(Resource* r);
        std::shared_ptr<ResHandle> find(Resource* r);
        void update(std::shared_ptr<ResHandle> handle);

        void freeOneResource();
        void memoryHasBeenFreed(uint32_t size);

    public:
        ResCache(const uint32_t sizeInMb, IResourceFile *file);
        virtual ~ResCache();

        bool init();

        void registerLoader(std::shared_ptr<IResourceLoader> loader);

        std::shared_ptr<ResHandle> getHandle(Resource* r);

        void Flush();
    };

} // namespace age::res