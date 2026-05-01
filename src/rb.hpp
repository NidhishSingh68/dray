#include <cstdint>

template <std::size_t size>
class rb{
  public:

    void push( std::uint8_t elm ){
      buffer[wp] = elm;
      wp = (wp + 1) % size;
    }

    int pop(){
      int ret = -1;
      if (rp != wp){
        ret = buffer[rp];
        rp = (rp + 1) % size;
      } 
      return ret;
    }

  private:
    std::uint8_t buffer[size];
    volatile std::uint32_t wp = 0; // Write Pointer
    volatile std::uint32_t rp = 0; // Read Pointer
};
