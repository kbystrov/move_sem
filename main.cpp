#include <iostream>

const size_t DEF_SIZE = 10;
#define prnt(msg) std::cout << msg << std::endl;

//!@class Vector class - class for data container
class Vector {
private:

    void swap(Vector& rhs) {

        prnt("Swap:\n\tthis = " << this << ";\n\trhs = " << &rhs);

        std::swap(capacity_, rhs.capacity_);
        std::swap(size_, rhs.size_);
        std::swap(data_, rhs.data_);
    }

public:

    size_t capacity_; ///< @param capacity_ The maximum number of elements in Vector
    size_t size_; ///< @param size_ The current number of filled elements in Vector
    int *data_; ///< @param data_ The pointer to elements array in Vector

    void DumpVector(const char *str) {
        if (str != nullptr) {
            std::cout << str << std::endl;
        }
        std::cout << "\tthis = " << this << std::endl;
        std::cout << "\tcapacity_ = " << capacity_ << std::endl;
        std::cout << "\tsize_ = " << size_ << std::endl;
        std::cout << "\tdata_ = " << data_ << std::endl;
    }

    Vector(): Vector(DEF_SIZE) {
    }

    explicit Vector(size_t capacity) :  ///< Parametrized constructor
            capacity_(capacity),
            size_(0),
            data_(new int[capacity]) {
        prnt("Vector(" << capacity << ") constructor:");
        DumpVector(nullptr);
    }

    ~Vector() {
        {
            DumpVector("~Vector() destructor:");
        }
        delete data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    } ///< Class destructor

    Vector(const Vector& rhs){

        prnt("Deep copy constructor: rhs =" << &rhs);

        new(this) Vector(rhs.capacity_);

        if (rhs.size_ > rhs.capacity_) {
            size_ = rhs.capacity_;
        } else {
            size_ = rhs.size_;
        }

        for (size_t i = 0; i < rhs.size_; i++) {
            data_[i] = rhs.data_[i];
        }
    }

    Vector& operator= (const Vector& rhs){

        prnt("Deep copy assignment: rhs = " << &rhs);

        if(this != &rhs) { ///< Preventing redundant copy in self-assignment operation
            this->~Vector();
            new(this) Vector(rhs);
        }

        return *this;
    }

    Vector(Vector&& rhs){

        prnt("Move constructor:\n\tthis =" << this <<  "\n\trhs = " << &rhs);

        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        data_ = rhs.data_;
        rhs.data_ = nullptr;
    }

    Vector& operator= (Vector&& rhs){

        prnt("Move assignment:\n\tthis = " << this << "\n\trhs = " << &rhs);

        if(this != &rhs) { ///< Preventing redundant swapping in self-assignment operation
            swap(rhs);
        }

        return *this;
    }

};

Vector operator+(const Vector& a, const Vector& b) {

    //try {
        prnt("Sum of " << &a << " + " << &b);

        int min_size = (a.size_ > b.size_) ? b.size_ : a.size_;

        Vector result(min_size);

        for (int i = 0; i < min_size; i++) {
            result.data_[i] = a.data_[i] + b.data_[i];
        }

        result.size_ = min_size;

        return result;
    /*} catch(...){
        prnt("Error");
        throw std::logic_error("Error");
    }*/
}

Vector ret_input(Vector input){
    prnt("In ret_input function: input = " << &input);
    return input;
}

Vector nrvo_func(){
    Vector result;
    return result;
}

Vector mod_nrvo_func(int flag){
    if(flag) {
        Vector result;
        return result;
    } else {
        Vector another;
        return another;
    }
}

Vector mod_rvo_func(int flag){
    if(flag) {
        return Vector(13);
    } else {
        return Vector(76);
    }
}

int main() {
    /*
    Vector v;
    Vector t(666);

    Vector c(v);
    c = t;
    */

    Vector a( Vector(15) ); //< Оптимизация: RVO в конструкторе

    prnt("A address = " << &a << "\n\n\n");

    Vector b = Vector(); //< Оптимизация: RVO в конструкторе

    prnt("B address = " << &b << "\n\n\n");

    b = Vector(17); //< Move assignment. No optimizations

    prnt("\n\n\n");

    Vector c = std::move(Vector(77)); //< Std::move. No optimizations

    prnt("C address = " << &c << "\n\n\n");

    a.size_ = 15;
    b.size_ = 17;

    Vector d = a + b; //< Два типа оптимизации: NRVO в операторе + и RVO в конструкторе

    prnt("D address = " << &d << "\n\n\n");

    c.size_ = 77;

    a = c + d; //< Оптимизация: NRVO в операторе +

    prnt("A address = " << &a << "\n\n\n");

    Vector e = ret_input(b); //< Оптимизация: RVO в конструкторе. NRVO нет, т.к. входной параметр

    prnt("E address = " << &e << "\n\n\n");

    c = ret_input(e); //< NRVO нет. Move assignment только.

    prnt("C address = " << &c << "\n\n\n");

    Vector f = nrvo_func(); // NRVO optimization in func + RVO in initialization

    prnt("F address = " << &f << "\n\n\n");

    d = nrvo_func(); // NRVO optimization in func

    prnt("D address = " << &d << "\n\n\n");

    Vector g = mod_nrvo_func(0); // No NRVO because of branching; ONLY RVO in constructor

    prnt("G address = " << &g << "\n\n\n");

    a = mod_nrvo_func(1); // No NRVO; Move constr + Move assignment

    prnt("A address = " << &a << "\n\n\n");

    srand ( time(NULL) );

    Vector h = mod_rvo_func(rand() % 2); // RVO in constructor + RVO in func

    prnt("H address = " << &h << "\n\n\n");

    g = mod_rvo_func( rand() % 2); // RVO in func + Move assignment

    prnt("G address = " << &g << "\n\n\n");

    return 0;
}
