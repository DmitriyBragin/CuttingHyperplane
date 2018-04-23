#pragma once
using namespace std;
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <conio.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <iostream>
//int simplexModule();

const double eps = 1e-10;


template<class T>
    bool eq_zero(T val)
{
    return fabs(val) < eps;
}

template<class T>
    bool ge(T val1, T val2)
{
    return val1 >= val2 - eps;
}

template<class T>
    bool le(T val1, T val2)
{
    return val1 <= val2 + eps;
}

template<class T>
    void swap(T & val1, T & val2)
{
    T t = val1;
    val1 = val2;
    val2 = t;
}


struct Indices : std::vector<int>
{
    Indices() {}
    Indices(int M) 
        : std::vector<int>(M)   
    {
        for(int i = 0; i < M; i++)
            (*this)[i] = i;
    }

    Indices(int M, int N) 
        : std::vector<int>(N - M)   
    {
        for(int i = 0; i < N - M; i++)
            (*this)[i] = M + i;
    }
    
    Indices operator-(Indices const& ind) const
    {
        Indices newInd = (*this);
        for(int i = 0; i < (int)ind.size(); i++)
            for(int j = 0; j < (int)newInd.size(); j++)
                if(newInd[j] == ind[i])
                    newInd.erase(newInd.begin() + j);
        return newInd;
    }

    Indices operator+(Indices const& ind) const
    {
        Indices newInd = (*this);
        for(int i = 0; i < (int)ind.size(); i++)
            newInd.push_back(ind[i]);
        return newInd;
    }
};

Indices ind(int i)
{
    Indices ii(1);    
    ii[0] = i;
    
    return ii;
}

template<class Type> 
    struct Matrix
{
public:
	Matrix() : n_(0), m_(0)  {}
    
    Matrix(Type * data, int M, int N = 1)
        : m_(M), n_(N)
    {
        data_.resize(M * N);
        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++)
                data_[to1d(i, j)] = data[to1d(i, j)];
    }

    Matrix(int M, int N = 1) : m_(M), n_(N)
    {
        data_.resize(M * N);
        for(int i = 0; i < M; i++)
            for(int j = 0; j < N; j++)
                data_[to1d(i, j)] = 0;
    }
    
    Type & operator() (int i, int j = 0) 
    { 
        assert(i < m_ && j < n_);
        return data_[to1d(i, j)]; 
    }
    Type const& operator() (int i, int j = 0) const 
    {
        assert(i < m_ && j < n_);
        return data_[to1d(i, j)]; 
    }
    int n() const { return n_; }
    int m() const { return m_; }
    
    void resize(int M, int N = 1)
    {           
        int i, j;              
        std::vector<Type> oldData = data_;        
        data_.resize(M * N);
        for(i = 0; i < m_; i++)
        {
            for(j = 0; j < n_; j++)
            {
                if(i < m_ && j < n_)
                    data_[to1d(N, i, j)] = oldData[to1d(i, j)];
                else
                    data_[to1d(N, i, j)] = 0;
            }
        }
                
        n_ = N;
        m_ = M;
    }
    
    void null()
    {
        for(int i = 0; i < m_; i++)
            for(int j = 0; j < n_; j++)
                data_[to1d(i, j)] = 0;
    }
    
    Matrix<Type> operator()(Indices const& ind1, Indices const& ind2) const
    {
        Matrix<Type> newM;
        newM.resize((int)ind1.size(), (int)ind2.size());
        for(int i = 0; i < (int)ind1.size(); i++)
            for(int j = 0; j < (int)ind2.size(); j++)
                newM(i, j) = (*this)(ind1[i], ind2[j]);
                
        return newM;
    }

    Matrix<Type> operator()(Indices const& ind1) const
    {
        Matrix<Type> newM;
        newM.resize((int)ind1.size());
        for(int i = 0; i < (int)ind1.size(); i++)
            newM(i) = (*this)(ind1[i]);
                
        return newM;
    }
    
    Matrix<Type> operator*(Matrix<Type> const& M) const
    {
        assert((*this).n() == M.m());
     
        Matrix<Type> MM;
        MM.resize((*this).m(), M.n());
        
        for(int i = 0; i < (*this).m(); i++)
            for(int j = 0; j < M.n(); j++)
            {
                MM(i, j) = 0;
                
                for(int k = 0; k < (*this).n(); k++)
                    MM(i, j) += (*this)(i, k) * M(k, j);
            }
        
        return MM;
    }
    
    Matrix<Type> operator-(Matrix<Type> const& M) const
    {
        Matrix<Type> MM = *this;
        return (MM -= M);
    }

    Matrix<Type> operator-() const
    {
        Matrix<Type> MM = *this;
        return MM * (-1);
    }
        
    Matrix<Type> & operator-=(Matrix<Type> const& M)
    {
        assert(m() == M.m() && n() == M.n());
        
        for(int i = 0; i < M.m(); i++)
            for(int j = 0; j < M.n(); j++)
                (*this)(i, j) -= M(i, j);
        
        return *this;
    }

        
    Matrix<Type> operator*(Type val) const
    {
        Matrix<Type> MM = *this;
        
        for(int i = 0; i < MM.m(); i++)
            for(int j = 0; j < MM.n(); j++)
                MM(i, j) *= val;
                
        return MM;
    }
    
    void set(Matrix<Type> const& M, Indices const& ind)
    {
        for(int i = 0; i < (int)ind.size(); i++)
            (*this)(ind[i]) = M(i);
    }

    void set(Type val, Indices const& ind)
    {
        for(int i = 0; i < (int)ind.size(); i++)
            (*this)(ind[i]) = val;
    }

    void set(Matrix<Type> const& M, Indices const& ind1, Indices const& ind2)
    {
        for(int i = 0; i < (int)ind1.size(); i++)
            for(int j = 0; j < (int)ind2.size(); j++)
                (*this)(ind1[i], ind2[j]) = M(i, j);
    }

    void set(Type val, Indices const& ind1, Indices const& ind2)
    {
        for(int i = 0; i < (int)ind1.size(); i++)
            for(int j = 0; j < (int)ind2.size(); j++)
                (*this)(ind1[i], ind2[j]) = val;
    }
   
    bool LI(int column0, int column1) const
    {
        for(int k = 0; k < m_; k++)
            for(int l = 0; l < m_; l++)
                if(!eq_zero((*this)(k, column0) * (*this)(l, column1) - 
                    (*this)(l, column0) * (*this)(k, column1)))
                {                           
                    return true;
                }
        
        return false;
    }
   
    bool LI(Indices const& ind, int column) const
    {
        for(int i = 0; i < (int)ind.size(); i++)
            if(!LI(ind[i], column))
                return false;
        
        return true;
    }
    
private:
    int to1d(int N, int i, int j) const { return i * N + j; }
    int to1d(int i, int j)        const { return to1d(n_, i, j); }

private:
    vector<Type> data_;
    int m_; // columns count
    int n_; // rows count
};


template<class Type>
    Matrix<Type> operator*(Type val, Matrix<Type> M)
{
    Matrix<Type> MM = M;
    
    for(int i = 0; i < MM.m(); i++)
        for(int j = 0; j < MM.n(); j++)
            MM(i, j) *= val;
            
    return MM;
}

template<class Type>
    Matrix<Type> transposed(Matrix<Type> const& M)
{
    Matrix<Type> MM(M.n(), M.m());
    for(int i = 0; i < M.m(); i++)    
        for(int j = 0; j < M.n(); j++)   
            MM(j, i) = M(i, j); 
            
    return MM;
}

template<class T, class Matrix>
    struct SwappingMatrix
{
    SwappingMatrix(Matrix & M) 
       : M_(M), ind1_(M.m()), ind2_(M.n()) 
    {}
    
    T & operator()(int i, int j)
    {
        return M_(ind1_[i], ind2_[j]);
    }

    T const& operator()(int i, int j) const
    {
        return M_(ind1_[i], ind2_[j]);
    }
    
    void swapRows(int i, int j)
    {
        int k = ind1_[i];
        ind1_[i] = ind1_[j];
        ind1_[j] = k;
    }
    
    void apply()
    {
        Matrix Mold = M_;
        for(int i = 0; i < m(); i++)
            for(int j = 0; j < n(); j++)
                M_(i, j) = Mold(ind1_[i], ind2_[j]);
    }

    int n() const { return M_.n(); }
    int m() const { return M_.m(); }
    
private:
    Matrix  &M_;
    Indices ind1_;
    Indices ind2_;
};


template<class Type> 
    void print(Matrix<Type> const& m)
{
    for(int i = 0; i < m.m(); i++)
    {
        printf("( ");
        for(int j = 0; j < m.n(); j++)
            printf("%5.1lf", m(i, j));
        printf(")\n");
    }

    //printf("\n");
}

template<class Type> 
void print(SwappingMatrix<Type, Matrix<Type> > const& m)
{
    for(int i = 0; i < m.m(); i++)
    {
        printf("( ");
        for(int j = 0; j < m.n(); j++)
            printf("%.1lf ", m(i, j));
        printf(")\n");
    }
    
    printf("\n");
}

void load(ifstream *f, int N, Matrix<double> & vec)
{
	vec.resize(N);
	double temp = 0;
	for (int i = 0; i < N; i++)
	{
		*f >> temp;
		vec(i) = temp;
	}
	// fscanf(f, "%lf", &vec(i));
}

void load(ifstream *f, int M, int N, Matrix<double> & A)
{
	A.resize(M, N);
	double temp = 0;
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
		{
			*f >> temp;
			A(i, j) = temp;
		}
	//  fscanf(f, "%lf", &A(i, j));
}

template<class T>
    bool inverse(Matrix<T> const& M2Inv, Matrix<T> &res)
{
    Matrix<T> M = M2Inv;
    M.resize(M2Inv.m(), M2Inv.n() * 2);
    for(int i = 0; i < M.m(); i++)
        for(int j = M2Inv.n(); j < M.n(); j++)
            if(i == j - M2Inv.n())
                M(i, j) = 1;
            else
                M(i, j) = 0;
    
    SwappingMatrix<T, Matrix<T> > swpM(M); 
    
    for(int i = 0; i < swpM.m(); i++)
    {
        T max = -1;
        int maxJ = -1;
        for(int j = i; j < swpM.m(); j++)
            if(fabs(swpM(j, i)) > max)
            {
                max = fabs(swpM(j, i));
                maxJ = j;
            }
            
        if(maxJ == -1 || eq_zero(max))
            return false;        
            
        swpM.swapRows(maxJ, i);

        for(int j = i + 1; j < swpM.m(); j++)
        {
            T d = swpM(j, i) / swpM(i, i);
            for(int k = i; k < swpM.n(); k++)
                swpM(j, k) -= swpM(i, k) * d;
        }
    }
    
    for (int i = swpM.m() - 1; i >= 0; i--) 
    {
        for (int j = i - 1; j >= 0; j--) 
        {
            T d = swpM(j, i) / swpM(i, i);
            for(int k = i; k < swpM.n(); k++)
                swpM(j, k) -= swpM(i, k) * d;
        }
    }

    for (int i = 0; i < swpM.m(); i++) 
    {
        T d = swpM(i, i);
        for(int k = 0; k < swpM.n(); k++)
            swpM(i, k) /= d;
    }
    
    swpM.apply();
    
    res = M(Indices(M2Inv.m()), Indices(M2Inv.n(), 2 * M2Inv.n()));
    
    return true;
}

template<class T>
    bool le(Matrix<T> const& M, T val)
{
    for(int i = 0; i < M.m(); i++)    
        for(int j = 0; j < M.n(); j++)
            if(M(i, j) > val + eps)
                return false;
                
    return true;
}

Matrix<double> identity_my(int m)
{
    Matrix<double> M(m, m);
    
    for(int i = 0; i < M.m(); i++)    
        for(int j = 0; j < M.n(); j++)
            if(i == j)
                M(i, j) = 1;
            else
                M(i, j) = 0;
                
    return M;
}
