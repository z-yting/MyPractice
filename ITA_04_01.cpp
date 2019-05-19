#include<iostream>
#include<array>
using std::array;
using std::cout;
using std::endl;
class Crange
{
public:
    int imaxLetf;
    int imaxRight;
};
int max(int a,int b)
{
    return a>b?a:b;
};
// low <= i <= mid < j < high
int findMaxCrossingSubarray(const int* adata,int low, int high, Crange& range)
{
    if(low == 0 && high == 4)
    {

    }

    int ileftSum = 0;
    int irightSum = 0;
    int isum;
    int mid;
    int i;
    bool flag;

    mid = (high + low)/2;
    range.imaxLetf = mid;
    range.imaxRight = mid + 1;

    flag = true;
    isum = 0;
    for(i = mid - 1; i >= low; i--)
    {
        flag = false;
        isum += adata[i];
        if(isum > ileftSum)
        {
            ileftSum = isum;
            range.imaxLetf = i;
        }
    }
    if(flag)
    {
        ileftSum = 0;
    }

    flag = true;
    isum =0;
    for ( i = mid + 1; i < high; i++)
    {
        flag = false;
        isum += adata[i];
        if(isum > irightSum)
        {
           irightSum = isum;
            range.imaxRight = i +1;
        }
    }
    if(flag)
    {
        irightSum = 0;
    }
    
    return adata[mid] + ileftSum + irightSum;
}
int maxSubArray(const int* adata,int low, int high,Crange& range)
{
    range.imaxLetf = low;
    range.imaxRight = high;

    if(low == high)
    {
        return INT32_MIN;
    }

    int mid;
    int iletfMax;
    int irightMax;
    int icrossingMax;
    Crange cleftRange;
    Crange crightRange;
    Crange ccrossRange;
    int imax = INT32_MIN;

    mid = (low + high)/2;

    cout<<low<<" ~ "<<mid<<" ~ "<<high<<endl;

    iletfMax = maxSubArray(adata,low,mid,cleftRange);
    irightMax = maxSubArray(adata,mid + 1,high,crightRange);
    //icrossingMax = findMaxCrossingSubarray(adata,low,high,ccrossRange);
    icrossingMax = findMaxCrossingSubarray(adata,low,high,ccrossRange);

    if(iletfMax > max(irightMax,icrossingMax))
    {
        imax = iletfMax;
        range = cleftRange;
    }
    else if(irightMax > icrossingMax)
    {
        imax = irightMax;
        range = crightRange;
    }
    else
    {
        imax = icrossingMax;
        range = ccrossRange;
    }
    
    return imax;
}
int main()
{
    Crange range;

    int a[] = {13,-3,-25,20,-3,-16,-23,18,20,-7,12,-5,-22,15,-4,7};
    int b[] = {1,-4,3,-6};
    int imax;

    imax =maxSubArray(a,0,sizeof(a)/sizeof(int),range);
    //imax =maxSubArray(b,0,sizeof(b)/sizeof(int),range);

    cout<<"max = "<<imax<<endl;


    cout<<"left index = "<<range.imaxLetf<<endl;
    cout<<"right index = "<<range.imaxRight<<endl;
    return 0;
}