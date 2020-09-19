#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * Generic Singleton template class
 */
template <typename T> class Singleton {
protected:
  Singleton() = default;
  Singleton(const Singleton &) = default;
  virtual ~Singleton() = default;
  static T *s_instance;

public:
  static T *getInstance();
  static void destroy();
};

template <typename T> T *Singleton<T>::s_instance = nullptr;

template <typename T> T *Singleton<T>::getInstance() {
  if (!Singleton<T>::s_instance)
    Singleton<T>::s_instance = static_cast<T *>(new T);
  return Singleton<T>::s_instance;
}

template <typename T> void Singleton<T>::destroy() {

  if (s_instance)
    delete s_instance;
  s_instance = nullptr;
}

#endif // SINGLETON_H
