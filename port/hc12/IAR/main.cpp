#include <algorithm>
#include <vector>
#include <functional>


template<typename T> class Max : public unary_function<T,bool> {
    T maximum;
  public:    
    Max() : maximum(0) {}
    void operator()(const T &val) 
    {
        if (val>maximum) {
            maximum=val;   
        }
    }
    T GetMaximum() const { return maximum; }
};

int main()
{
    int arr[]={34,6,677,444,1,8,9,4,2,644,75,34};
    const int arr_size=sizeof(arr)/sizeof(int);
    int maximum;
    
    Max<int> max=for_each(arr,arr+arr_size,Max<int>());
    maximum=max.GetMaximum();
    
//    sort(arr,arr+arr_size);
    
    return 0;
}
