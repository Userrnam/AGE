import os
import shutil


INCLUDE_PATH = "/usr/local/include/"
LIBRARY_PATH = "/usr/local/lib/"


def build():
    if not os.access(".build", os.O_RDONLY):
        os.mkdir(".build")
    
    os.chdir(".build")
    os.system("cmake ..")
    os.system("cmake --build .")

def remove_prefix(s, prefix):
    if s.startswith(prefix):
        return s[len(prefix):]
    return s

def path_check(path):
    p = ''
    if path[0] == '/':
        p = '/'
        path = path[1:]
    
    l = path.split('/')
    if len(l) == 1:
        return
    
    l = l[:-1]
    for d in l:
        p = p + d + '/'
        if os.path.isdir(p):
            continue
        os.mkdir(p)

# requires full pathes in files
def copy_module_headers(path, module_name, files):
    module_path = path + module_name
    if not os.access(module_path, os.O_RDWR):
        os.mkdir(module_path)

    with open(path + module_name + '.hpp', 'w') as module_header:
        for _f in files:
            f = 'AGE/src/' + module_name + '/' + _f
            p = f.replace('AGE/src/', path)
            path_check(p)
            shutil.copy2(f, p)
            relative_path = remove_prefix(f, 'AGE/src/')
            module_header.write('#include <AGE/' + relative_path + '>\n')

def install(include_path, lib_path):
    # check directories exist
    if not os.access(include_path, os.O_RDWR):
        print("cannot access " + include_path)
        return -1
    
    if not os.access(lib_path, os.O_RDWR):
        print("cannot access " + lib_path)
        return -1

    # copy library
    shutil.copy2('.build/AGE/src/libage.a', lib_path)

    if include_path[-1] != '/':
        include_path += '/'

    include_path += 'AGE/'
    if not os.access(include_path, os.O_RDWR):
        os.mkdir(include_path)

    # create includes
    copy_module_headers(include_path, 'Animation', [
        'Animation.hpp',
        'AnimationState.hpp',
        'Animator.hpp',
        'LinearFunction.hpp',
        'StepFunction.hpp',
        "TransformResolveStructure.hpp"
    ])

    copy_module_headers(include_path, 'Audio', [
        'Listener.hpp',
        'Sound.hpp',
        'SoundSource.hpp'
    ])

    copy_module_headers(include_path, 'Containers', [
        'Arena.hpp',
        'Tuple.hpp',
        'TemplateUtils.hpp',
        'TaggedUnion.hpp',
        'DefaultAllocator.hpp',
        'DynamicArray.hpp'
    ])

    copy_module_headers(include_path, 'Events', [
        'Cursor.hpp',
        'Event.hpp',
        'EventManager.hpp'
    ])

    copy_module_headers(include_path, 'External', [
        'entt.hpp'
    ])

    copy_module_headers(include_path, 'Graphics', [
        'PositionManager.hpp',
        'Transformable.hpp',
        'UnmanagedTransformable.hpp',
        'MemoryHolders/Buffer.hpp',
        'MemoryHolders/Image.hpp',
        "ObjectCreation/Components/ArrayComponent.hpp",
        "ObjectCreation/Components/BundleComponent.hpp",
        "ObjectCreation/Components/ColorComponent.hpp",
        "ObjectCreation/Components/ComponentCollector.hpp",
        "ObjectCreation/Components/FontComponent.hpp",
        "ObjectCreation/Components/GetSet.hpp",
        "ObjectCreation/Components/ShaderComponent.hpp",
        "ObjectCreation/Components/StorageComponent.hpp",
        "ObjectCreation/Components/TexCoordsComponent.hpp",
        "ObjectCreation/Components/TextComponent.hpp",
        "ObjectCreation/Components/TextureComponent.hpp",
        "ObjectCreation/Components/TileMapComponent.hpp",
        "ObjectCreation/Components/TransformComponent.hpp",
        # "Core/CoreConfiguration.hpp",
        "ObjectCreation/Descriptor.hpp",
        "ObjectCreation/Drawable.hpp",
        "ObjectCreation/Shader.hpp",
        "ObjectCreation/ShapeManager.hpp",
        "ObjectCreation/Index.hpp",
        "Memory/MemoryId.hpp",
        "Objects/Sampler.hpp",
        "Objects/Texture.hpp",
        "View/Camera.hpp",
        "View/View.hpp",
    ])

    copy_module_headers(include_path, 'Math', [
        'Vector.hpp'
    ])

    copy_module_headers(include_path, 'Scene', [
        'Entity.hpp',
        'Scene.hpp',
        'SceneBase.hpp',
        'ScriptComponent.hpp',
        'Application.hpp'
    ])

    copy_module_headers(include_path, 'UI', [
        'IButton.hpp',
        'UIBlock.hpp',
        'UIManager.hpp'
    ])

    copy_module_headers(include_path, 'Utils', [
        'utils.hpp'
    ])

if __name__ == '__main__':
    install(INCLUDE_PATH, LIBRARY_PATH)
