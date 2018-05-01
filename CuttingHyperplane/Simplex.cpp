#include "matrix.h"
#include "main.h"
using namespace std;
template<class T>
    struct Simlex
{
    enum
    {
        INFINITE,
        SUCCESS,
        FAIL
    };

    Simlex(Matrix<T> const& A, Matrix<T> const& b, Matrix<T> const& c)
    {
        A_ = A, b_ = b, c_ = c;              
        n_ = A_.n();
        m_ = A_.m();
    }

    Matrix<T> operator()(Matrix<T> const& x)
    {
        x_ = x;
        
        for(int i = 0; i < 100; i++)                
        {
           // printf("#%d", i);
           // print(transposed(x_));
           // printf("f = %lf\n", (transposed(c_) * x_)(0, 0));
            
            int res = makeStep();
            if(res == INFINITE)
            {
                printf("Not limited\n");
                break;
            }
            else if(res == SUCCESS)
            {
                break;
            }
        }
        
     //   printf("\nsolved = %lf\n", (transposed(c_) * x_)(0, 0));
        return x_;
    }

    Matrix<T> operator()()
    {   
     //   printf("A\n");
    //    print(A_);
    //    printf("b\n");
    //    print(transposed(b_));
    //    printf("c\n");
    //    print(transposed(c_));
     //   printf("\n");
    
   //     printf("\ninitial vector\n");
		if (flag == true)
		{
			x_ = calcInitialVector();
			flag = false;
		}
		else
		{
			vector<double> solutCoeffTemp = solutionLastStep.getData();
			solutCoeffTemp.resize(solutCoeffTemp.size() + 1);
			Matrix<T> newVec;

		}
		/* “”“ Õ¿ƒŒ ¬Ã≈—“Œ  ¿À ¬≈ “Œ– ƒ¿“‹ «Õ¿◊≈Õ»≈ œ–≈ƒ€ƒ”Ÿ≈√Œ ÿ¿√¿ */
     //  printf("\ntask\n");

        operator()(x_);
		solutionLastStep = x_;
		return y_dualSolution;
        return x_;
    }

	void setFlag(bool _flag) { flag = _flag; }
    void makeDual()
    {
        Matrix<T> A = transposed(A_);
        Matrix<T> c = c_;
        Matrix<T> b = b_;
        
        A_.resize(n_, n_ + 2 * m_);
        A_.set(A, Indices(0, n_), Indices(0, m_));
        A_.set(-A, Indices(0, n_), Indices(m_, 2 * m_));
        A_.set(identity_my(n_), Indices(0, n_), Indices(2 * m_, 2 * m_ + n_));

        c_.resize(n_ + 2 * m_);
        c_.set(-b, Indices(0, m_));
        c_.set(b, Indices(m_, 2 * m_));
        c_.set(0, Indices(2 * m_, 2 * m_ + n_));
        
        b_ = c;
        
        n_ = A_.n();
        m_ = A_.m();
    }
    
private:
    Matrix<T> calcInitialVector()
    {
        Matrix<T> AA = A_;
        Matrix<T> c(n_ + m_);
        Matrix<T> x(n_ + m_);
        Matrix<T> b = b_; 
        
        
        for(int i = 0; i < m_; i++)
        {
            if(b(i) < 0)
            {
                b(i) *= -1;
                for(int j = 0; j < n_; j++)
                    AA(i, j) *= -1;
            }
        } 
        
        AA.resize(m_, n_ + m_);
        for(int i = 0; i < m_; i++)
            for(int j = 0; j < m_; j++)
                if(i == j)
                    AA(i, n_ + j) = 1;
                else
                    AA(i, n_ + j) = 0;
                

        x.set(b, Indices(n_, n_ + m_));
        c.set(1, Indices(n_, n_ + m_));

        x = Simlex<T>(AA, b, c) (x);
        
        return x(Indices(n_));
    }
    
    int makeStep()
    {
        Indices M(m_);
    
        // fill N+
        Nplus_.clear();
        for(int i = 0; i < n_; i++)
            if(!eq_zero(x_(i)))
                Nplus_.push_back(i);
        N0_ = Indices(n_) - Nplus_;
    
        addN_.clear();
        
        if(Nplus_.size() != m_)
            makeN(Indices(), 0);
        else
            addN_.push_back(Indices());
        
        for(int i = 0; i < (int)addN_.size(); i++)
        {
            N_ = Nplus_ + addN_[i];
            
            Indices L = Indices(n_) - N_;

            if(!inverse(A_(M, N_), B_))
                continue;

            //printf("\n A : \n");
            //print(A_);
            //printf("\n A * x :  \n");
            //print(transposed(A_ * x_));

            //printf("\n B * A :  \n");
            //print(B_ * A_(M, N_));

            //printf("\n B :  \n");
            //print(B_);

            Matrix<T> y = transposed(c_(N_)) * B_;
			y_dualSolution = transposed(c_(N_)) * B_;
            //printf("\n c_(N_) :");
            //print(c_(N_));

            /*printf("\n y :");
            print(y);
            
            printf("\n A : \n");
            print(A_(M, L));
            
            printf("\n c :");
            print(c_(L));
            */
            
            d_ = transposed(transposed(c_(L)) - y * A_(M, L));

            //printf("\n d :  ");
            //print(transposed(d_));
           
			int j = 0;
            for(j = 0; j < d_.m(); j++)
                if(!ge(d_(j), 0.))
                //if(d_(j) < 0)
                    break;
                
			if (j == d_.m())
			{
			//	cout << "Dual task solution:" << endl;
		//		print(y_dualSolution);
				return SUCCESS; // problem have been solved, x_ is optimal vector
			}
            j = L[j];
                        
            // u_
            u_.resize(n_);
            u_.null();
            u_.set(B_ * A_(M, ind(j)), N_);
            u_(j) = -1;
           
            
            //printf("\n u :");
            //print(transposed(u_));
            
           
            //if(le(u_(N_), 0.))
              //  return INFINITE; // not limited
            
            if(Nplus_ == N_ || le(u_(N_ - Nplus_), 0.))
            { 
                double theta = -1;
                for(int i = 0; i < (int)N_.size(); i++)
                {
                    int j = N_[i];
                    if(!le(u_(j), 0.) && (theta < 0 || x_(j) / u_(j) < theta))
                        theta = x_(j) / u_(j);                    
                }
                
                if(theta < 0)
                    return INFINITE; // not limited
                
                x_ -= theta * u_;
                break;
            }
        }
        
           
        return FAIL;
    }
    void makeN(Indices N, int i)
    {
        for(; i < (int)N0_.size(); i++)
        {
            int j = N0_[i];
                
            if(eq_zero(x_(j)) && A_.LI(Nplus_ + N, j))
            {
                if(N.size() == m_ - 1 - Nplus_.size())
                    addN_.push_back(N + ind(j));
                else
                    makeN(N + ind(j), i + 1);
            }
        }
    }
    
private:
    Matrix<T> A_;
    Matrix<T> b_;
    Matrix<T> c_;
    
    Matrix<T> B_;
    Matrix<T> x_;
    Matrix<T> y_;
    Indices N_, Nplus_, N0_;
    std::vector<Indices> addN_;
    Matrix<T> d_;
    Matrix<T> u_;
	Matrix<T> y_dualSolution;
	Matrix<T> solutionLastStep;
	bool flag;
    int n_;
    int m_;
};




vector<double> simplexModule()
{
    // load data
	ifstream f;
	vector<double> result;
	f.open("inputDUAL.txt");

	 /* Reading size of matrixes*/
	int N, M;
	f >> M;
	f >> N;
    
   

    Matrix<double> A;
    Matrix<double> b, c;
	Matrix<double> solution;

    load(&f, M, N, A);        
    load(&f, M, b);        
    load(&f, N, c);        
	f.close();




	Simlex<double> simplex(A, b, c);
    // solve
	simplex.setFlag(true);
    solution = simplex();
	result = solution.getData();
	for (int i = 0; i < result.size(); i++)
	{
		result[i] *= -1; /* ¬Õ»Ã¿Õ»≈, Ã»Õ”— —“Œ»“ »«-«¿ ƒ¬Œ…—“¬≈ÕÕŒ… «¿ƒ¿◊» »—œ–¿¬»“‹ ≈—À» Õ”∆Õ¿ œ–ﬂÃ¿ﬂ*/
	}
	return result;
	printf("\ndual\n");
	Simlex<double> simplex2(A, b, c);
	simplex2.makeDual();

	solution = simplex2();
	result.resize(solution.m());
	for (int i = 0; i < solution.m(); i++)
	{
		result[i] = solution(i);
	}



	return result;
}

