template<class SINGLE> class Singleton {
 public:
  static SINGLE* Instance(void);
 protected:
  Singleton(void);
 private: 
  static SINGLE* _instance;
};

template<class SINGLE>
SINGLE * Singleton<SINGLE>::_instance = NULL;

template<class SINGLE>
Singleton<SINGLE>::Singleton(void) {}

template<class SINGLE>
SINGLE * Singleton<SINGLE>::Instance(void) {
  if (_instance == NULL) {
    _instance = new Singleton;
  }
  return _instance;
}
