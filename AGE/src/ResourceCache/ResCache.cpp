#include "ResCache.hpp"

namespace age::res {

    bool wildcardMatch(const char *pat, const char *str) {
        int i, star;

        new_segment:

        star = 0;
        if (*pat == '*') {
            star = 1;
            do { pat++; } while (*pat == '*');
        }

        test_match:

        for (i = 0; pat[i] && (pat[i] != '*'); i++) {
            //if (mapCaseTable[str[i]] != mapCaseTable[pat[i]]) {
            if (str[i] != pat[i]) {
                if (!str[i]) return 0;
                if ((pat[i] == '?') && (str[i] != '.')) continue;
                if (!star) return 0;
                str++;
                goto test_match;
            }
        }
        if (pat[i] == '*') {
            str += i;
            pat += i;
            goto new_segment;
        }
        if (!str[i]) return 1;
        if (i && pat[i - 1] == '*') return 1;
        if (!star) return 0;
        str++;
        goto test_match;
    }

    Resource::Resource(const std::string& name) {
        m_name = name;
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), (int(*)(int)) std::tolower);
    }

    ResHandle::ResHandle(Resource& resource, char* buffer, uint32_t size, ResCache* pResCache) : m_resource(resource) {
        m_buffer = buffer;
        m_size = size;
        m_extra = NULL;
        m_pResCache = pResCache;
    }

    ResHandle::~ResHandle() {
        delete [] m_buffer;
        m_pResCache->memoryHasBeenFreed(m_size);
    }

    ResCache::ResCache(const uint32_t sizeInMb, IResourceFile* resFile) {
        m_cacheSize = sizeInMb * 1024 * 1024;
        m_allocated = 0;
        m_file = resFile;
    }

    ResCache::~ResCache() {
        while (!m_resHandleList.empty()) {
            freeOneResource();
        }
        delete m_file;
    }

    bool ResCache::init() {
        bool result = false;
        if (m_file->vOpen()) {
            registerLoader(std::shared_ptr<IResourceLoader>(new DefaultResourceLoader()));
            result = true;
        }
        return result;
    }

    void ResCache::registerLoader(std::shared_ptr<IResourceLoader> loader) {
        m_resourceLoaders.push_front(loader);
    }

    std::shared_ptr<ResHandle> ResCache::getHandle(Resource* r) {
        std::shared_ptr<ResHandle> handle(find(r));
        if (handle == NULL) {
            handle = load(r);
        }
        else {
            update(handle);
        }
        return handle;
    }

    std::shared_ptr<ResHandle> ResCache::load(Resource* r) {
        std::shared_ptr<IResourceLoader> loader;
        std::shared_ptr<ResHandle> handle;

        for (ResourceLoaders::iterator it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); ++it) {
            std::shared_ptr<IResourceLoader> testLoader = *it;

            if (wildcardMatch(testLoader->vGetPattern().c_str(), r->m_name.c_str())) {
                loader = testLoader;
                break;
            }
        }

        if (!loader) {
            return handle;
        }

        int rawSize = m_file->vGetRawResourceSize(*r);
        if (rawSize < 0) {
            return std::shared_ptr<ResHandle>();
        }

        int allocSize = rawSize;
        char* rawBuffer = loader->vUseRawFile() ? allocate(allocSize) : new char[allocSize];
        memset(rawBuffer, 0, allocSize);

        if (rawBuffer == NULL || m_file->vGetRawResource(*r, rawBuffer) == 0) {
            return std::shared_ptr<ResHandle>();
        }

        char* buffer = NULL;
        uint32_t size = 0;

        if (loader->vUseRawFile()) {
            buffer = rawBuffer;
            handle = std::shared_ptr<ResHandle>(new ResHandle(*r, buffer, rawSize, this));
        }
        else {
            size = loader->vGetLoadedResourceSize(rawBuffer, rawSize);
            buffer = allocate(size);
            if (rawBuffer == NULL || buffer == NULL) {
                return std::shared_ptr<ResHandle>();
            }
            handle = std::shared_ptr<ResHandle>(new ResHandle(*r, buffer, size, this));
            bool success = loader->vLoadResource(rawBuffer, rawSize, handle);

            if (!success) {
                return std::shared_ptr<ResHandle>();
            }
        }

        if (handle) {
            m_resHandleList.push_front(handle);
            m_resources[r->m_name] = handle;
        }

        return handle; 
    }

    std::shared_ptr<ResHandle> ResCache::find(Resource* r) {
        ResHandleMap::iterator it = m_resources.find(r->m_name);
        if (it == m_resources.end()) {
            return std::shared_ptr<ResHandle>();
        }
        return it->second;
    }

    void ResCache::update(std::shared_ptr<ResHandle> handle) {
        m_resHandleList.remove(handle);
        m_resHandleList.push_front(handle);
    }

    char* ResCache::allocate(uint32_t size) {
        if (!makeRoom(size)) {
            return NULL;
        }

        char* mem = new char[size];
        if (mem) {
            m_allocated += size;
        }
        return mem;
    }

    void ResCache::freeOneResource() {
        ResHandleList::iterator it = m_resHandleList.end();
        it--;

        std::shared_ptr<ResHandle> handle = *it;

        m_resHandleList.pop_back();
        m_resources.erase(handle->m_resource.m_name);
    }

    void ResCache::Flush() {
        while (!m_resHandleList.empty()) {
            std::shared_ptr<ResHandle> handle = *(m_resHandleList.begin());
            // free(handle); // Red flag
            m_resHandleList.pop_front;
        }
    }

    bool ResCache::makeRoom(uint32_t size) {
        if (size > m_cacheSize) {
            return false;
        }

        while (size > (m_cacheSize - m_allocated)) {
            if (m_resHandleList.empty()) {
                return false;
            }
            freeOneResource();
        }

        return true;
    }

    void ResCache::free(std::shared_ptr<ResHandle> pHandle) {
        m_resHandleList.remove(pHandle);
        m_resources.erase(pHandle->m_resource.m_name);
        //m_allocated -= pHandle->m_resource.m_size;
        //delete pHandle;
    }

    void ResCache::memoryHasBeenFreed(uint32_t size) {
        m_allocated -= size;
    }

} // namespace age::res