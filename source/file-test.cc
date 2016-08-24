#include<fstream>
#include<vector>

int main(void) {
  vector<unsigned int> v;

  std::ifstream input("/tmp/test");
  if (!input) {
    cerr << "file error" << endl;
    exit(-1);
  }
  for(unsigned int i = 0; i < 3; i++) {
    unsigned int aux;
    input >> aux;
    v.push_back(aux);
  }

  for(unsigned int i = 0; i < 3; i++)
    cout << v[i] << " ";
  cout << endl;

  return 0;
}
