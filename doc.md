# Creating simple Appliction.
Let's start by creating our Application class, that derives from `age::Application`

```C++
// age::Application is declared in Scene.hpp, so we include it (we could also include Scene/Application.hpp)
#include <AGE/Scene.hpp>

struct MyApplication : public age::Application {
    MyApplication(const std::string& name, int width, int height)
        : age::Application(name, width, height) {}
};
```

Now we can create our Application in main:

```C++
int main() {
    MyApplication* app = new MyApplication("my app", 800, 600);
    app->run();
    delete app;
    return 0;
}
```

If you compile and run it will say that it needs a scene so, let's create one

```C++
struct MyScene : public age::Scene {
    MyScene(age::Application* app) : age::Scene(app) {}
};
```

And update MyApplication constructor
```C++
    MyApplication(const std::string& name, int width, int height)
        : age::Application(name, width, height) {
        selectScene<MyScene>();
    }
```

Now everything works fine.

## Creating a Rectangle
For rendering objects AGE uses Drawables. Drawables are described by their shape and GraphicsComponents. GraphicsComponents tell the API what properties the drawable has (like Color, texture, transform, etc).

Now let's render a rectangle on the screen. In AGE in order to create an object you need to create a script that controlls it.

Classes that we will be using:
1. `age::ScriptComponent` -- script component that is required to create an object that can be handled by `age::Scene`
2. `age::Transformable` -- class, that holds current transform of the object
3. `age::StorageComponent<Args..>` -- buffer that will be used for rendering
4. `age::Transform` -- property of drawable object, that contains transformation matrix, so the rectangle could be moved, scaled and rotated
5. `age::Drawable` -- the ojbect that will be used for rendering

```C++
struct MyRectangle : public age::ScriptComponent {
    age::Transformable transformable;
    age::StorageComponent<age::Transform> buffer;

    MyRectangle(Entity e) : age::ScriptComponent(e) {
        transformable.create(e);
        transformable.setScale(100, 100);
        auto transform = transformable::getTransform();
        buffer.create();
        buffer.set(transform);
        
        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            staticView(),
            buffer
        );
    }
    
    ~MyRectangle() {
        transformable.destroy();
        buffer.destroy();
        getComponent<age::Drawable>().destroy();
    }
};
```

Now, we can add rectangle to the scene:
```C++
struct MyScene : public age::Scene {
    MyScene(age::Application* app) : age::Scene(app) {
        createEntity<MyRectangle>();
    }
};
```

Now you shold be able to see white rectangle in the bottom left corner of the screen

## Moving a rectangle

Let's create a function that will be called each frame
```C++
void update(const age::event::Update& e) {
```

Find dx and dy
```C++
    float speed = 250.0f;
    auto dx = (age::isKeyPressed(GLFW_KEY_RIGHT) - age::isKeyPressed(GLFW_KEY_LEFT)) * speed * e.elapsedTime;
    auto dy = (age::isKeyPressed(GLFW_KEY_UP) - age::isKeyPressed(GLFW_KEY_DOWN)) * speed * e.elapsedTime;
```

Move transformable
```C++
    transformable.move(dx, dy);
```

And upload changes
```C++
    buffer.set(transformable.getTransform());
}
```

Now we have to tell AGE about this method using `EVENT_CALLBACK` macro
```C++
struct MyRectangle : public age::ScriptComponent {
    EVENT_CALLBACK(MyRectangle, update);
```

Here we use macro `EVENT_CALLBACK` that tells the API about our callback. The first parameter to the macro is
name of the class, the second parameter is the callback method. Callback type is determined by the method parameter.

## Changing RectangleColor

To change color we can add `age::Color` to the `age::StorageComponent`
```C++
    age::StorageComponent<age::Transform, age::Color> buffer;
```

When we `age::StorageComponent` has more than 1 component:
1. buffer must be uploaded explicitly using `upload` method
```C++
    buffer.upload();
```
2. when we set values we must specify what we set using template parameter
```C++
    buffer.set<age::Transform>(transformable.getTransform());
```

Now we can change color of rectangle
```C++
    buffer.set<age::Color>(age::Color(1.0f, 0.0f, 0.0f, 1.0f));
```
