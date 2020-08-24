#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intal.h"

static char* find_fibonacci(unsigned int n, char** fib);
static void intal_merge_sort(char** arr,int start, int end);
static void intal_merge(char** arr,int start, int mid, int end);
static char* intal_half(const char* intal);

// Returns the sum of two intals.
// Each digit is summed from the unit's place and the carry is propagated to the next places.
char* intal_add(const char* intal1, const char* intal2)
{
	int l1 = strlen(intal1);
	int l2 = strlen(intal2);
	int len = (l1>l2)?l1:l2;
	
	char* result = malloc((len+2) * sizeof(char));
	result[len+1] = '\0';
	
	
	int i1 = l1-1;
	int i2 = l2-1;
	int i = len;
	
	int sum = 0;
	int carry = 0;
	
	while((i2>=0) && (i1>=0))
	{
		sum = (intal1[i1] - '0') + (intal2[i2] - '0') + carry;
		result[i] = sum%10 + '0';
		carry = sum/10;
		
		--i;
		--i1;
		--i2;
	}
	
	if(i2 == -1)
	{
		while(i1>=0)
		{
			sum = intal1[i1] - '0' + carry;
			result[i] = sum%10 + '0';
			carry = sum/10;
			
			--i;
			--i1;			
		}
	}
	
	else
	{
		while(i2>=0)
		{
			sum = intal2[i2] - '0' + carry;
			result[i] = sum%10 + '0';
			carry = sum/10;
			
			--i;
			--i2;			
		}
	}
	
	if(carry != 0)
	{
		result[0] = carry + '0';
		return result;
	}
	
	char* re_result = malloc((len+1) * sizeof(char));
	memcpy(re_result,result+sizeof(char),(len+1) * sizeof(char));
	free(result);
	return re_result;
}

// Returns 0 when both are equal.
// Returns +1 when intal1 is greater, and -1 when intal2 is greater.
// Function initially compares the length to return the appropriate result.
// If the lengths are equal, each digit is compared from the start to find the largest intal.
int intal_compare(const char* intal1, const char* intal2)
{
	int l1 = strlen(intal1);
	int l2 = strlen(intal2);
	
	if(l1 > l2) return 1;
	if(l2 > l1) return -1;
	
	int i;
	for(i = 0; i<l1 && intal1[i] == intal2[i]; ++i);
	if(i==l1) return 0;
	if(intal1[i] > intal2[i]) return 1;
	return -1;
}

// Returns the difference (obviously, nonnegative) of two intals.
// Implementation is similar to intal_add() where the carry is propagated to the next place.
char* intal_diff(const char* intal1, const char* intal2)
{
	int comp = intal_compare(intal1,intal2);
	
	if(comp == 0)
	{
		char* result = malloc(2*sizeof(char));
		result[0] = '0';
		result[1] = '\0';
		return result;
	}	
	
	char* a = intal1;
	char* b = intal2;
	if(comp == -1)
	{
		b = intal1;
		a = intal2;
	}
	
	int l1 = strlen(a);
	int l2 = strlen(b);
	
	char* result = malloc((l1+1) * sizeof(char));
	result[l1] = '\0';

	int i1 = l1-1;
	int i2 = l2-1;
	int i = l1-1;

	int diff = 0;
	int carry = 0;

	while((i2>=0) && (i1>=0))
	{
		diff = (a[i1] - '0') - (b[i2] - '0') - carry;
		if(diff < 0)
		{
			result[i] = 10 + diff + '0';
			carry = 1;
		}
		else
		{
			result[i] = diff + '0';
			carry = 0;
		}
		
		--i;
		--i1;
		--i2;
	}
	
	while(i1>=0)
	{
		diff = (a[i1] - '0') - carry;
		if(diff < 0)
		{
			result[i] = 10 + diff + '0';
			carry = 1;
		}
		else
		{
			result[i] = diff + '0';
			carry = 0;
		}
		
		--i;
		--i1;		
	}
	
	int it = 0;
	while(result[it] == '0') ++it;
	
	if(it == 0) return result;	
	char* re_result = malloc((l1+1-it)*sizeof(char));
	
	memcpy(re_result,result+(it*sizeof(char)),(l1+1-it) * sizeof(char));
	free(result);
	
	return re_result;

}

// Returns the product of two intals.
// The approach is similar to the school approach of multiplying 2 integers.
// There are 2 loops, one each to traverse the given intals.
// In each iteration of the inner loop, one digit of intal1 is multiplied with intal2 and the product is stored.
// This product is added to the padded values of the next inner loop iterations to produce the final porduct.
char* intal_multiply(const char* intal1, const char* intal2)
{
	int l1 = strlen(intal1);
	int l2 = strlen(intal2);
	
	if(l1==1 || l2==1)
	{
		if(intal1[0] == '0' || intal2[0] == '0')
		{
			char* result = malloc(2*sizeof(char));
			result[0] = '0';
			result[1] = '\0';
			return result;			
		}
	}
	
	
	char* result = malloc((l1+l2+1)*sizeof(char));
	result[l1+l2] = '\0';
	for(int i = 0; i<(l1+l2); ++i) result[i] = '0';
	
	
	int n = l1+l2-1;
	int i1 = 0;
	int i2 = 0;
	
	int carry = 0;
	int sum = 0;
	
	for(int i = l2-1; i>=0; --i)
	{
		i2 = 0;
		carry = 0;
		int a = intal2[i] - '0';
		
		for(int j = l1-1; j>=0; --j)
		{
			int b = intal1[j] - '0';
			
			sum = a*b + (result[n-i1-i2] - '0') + carry;
			
			carry = sum/10;
			result[n-i1-i2] = (sum%10) + '0';
			++i2;
		}
		
		if(carry != 0)
		{
			result[n-i1-i2] += carry;
		}
		++i1;
	}
	
	int it = 0;
	while(result[it] == '0') ++it;
	
	if(it == 0) return result;	
	char* re_result = malloc((l1+l2+1-it)*sizeof(char));
	
	memcpy(re_result,result+(it*sizeof(char)),(l1+l2+1-it) * sizeof(char));
	free(result);
	
	return re_result;	
}

// Returns value of intal/2
static char* intal_half(const char* intal)
{
	if(strlen(intal) == 1 && intal[0] == '1')
	{	
		char* result = malloc(2*sizeof(char));
		result[0] = '0';
		result[1] = '\0';
		return result;
	}
	int len = strlen(intal);
	char* result = malloc((len+1)*sizeof(char));
	result[len] = '\0';

	int remainder = 0;
	int quotient;
	int i;
	for(i = 0; i<len; ++i)
	{
		int numb = remainder*10 + (intal[i] - '0');
		result[i] = numb/2 + '0';
		remainder = numb%2;
	}
	int it = 0;
	while(result[it] == '0') ++it;

	if(it == 0) return result;
	char* re_result = malloc(len*sizeof(char));

	memcpy(re_result,result+(it*sizeof(char)),len * sizeof(char));
	free(result);

	return re_result;
}

// Returns intal1 mod intal2
// The mod value should be in the range [0, intal2 - 1].
// intal2 > 1
// Implement a O(log intal1) time taking algorithm.
// O(intal1 / intal2) time taking algorithm may exceed time limit.
// O(intal1 / intal2) algorithm may repeatedly subtract intal2 from intal1.
// That will take intal1/intal2 iterations.
// You need to design a O(log intal1) time taking algorithm.
// Generate your own testcases at https://www.omnicalculator.com/math/modulo
// We divide intal1 to find the nearest multiple to intal2, remainder is finally returned.
char* intal_mod(const char* intal1, const char* intal2)
{
   	int cmp;
   	cmp = intal_compare(intal1,intal2);
   	if(cmp == 0)
   	{
		char* result = malloc(2*sizeof(char));
		result[0] = '0';
		result[1] = '\0';
   		return result;
   	}
	char* low = malloc(2*sizeof(char));
	low[0] = '0';
	low[1] = '\0';

	char* high = malloc((strlen(intal1)+1)*sizeof(char));
	memcpy(high,intal1,(strlen(intal1)+1)*sizeof(char));

	char* sum;char* mid;char* product;char* diff;
	while(intal_compare(low,high) == -1)
	{
		sum = intal_add(low,high);
		mid = intal_half(sum);
		free(sum);

		product = intal_multiply(intal2,mid);

		cmp = intal_compare(intal1,product);
		if(cmp == 1)
		{
			diff = intal_diff(intal1,product);
			cmp = intal_compare(diff,intal2);
			free(diff);
		}
		else cmp = -1;
		if(cmp > 0)
		{
			free(low);
			low = intal_add(mid,"1");
			free(mid);			
		}
		else
		{
			free(high);
			high = mid;
		}
		free(product);
	}
	product = intal_multiply(low,intal2);
	diff = intal_diff(intal1,product);

	return diff;
}

// Returns intal1 ^ intal2.
// Let 0 ^ n = 0, where n is an intal.
// Implement a O(log n) intal multiplications algorithm.
// 2^3000 has less than 1000 decimal digits. 3000 intal multiplications may exceed time limit.
// Decrease and conquer method is used to reduce the problem statement.
char* intal_pow(const char* intal1, unsigned int n)
{
	if(n==0)
	{
		char* result = malloc(2*sizeof(char));
		result[0] = '1';
		result[1] = '\0';
		
		return result;		
	}
	if(n==1)
	{
		char* result = malloc((strlen(intal1)+1)*sizeof(char));
		memcpy(result,intal1,(strlen(intal1)+1)*sizeof(char));
		return result;
	}
	
	unsigned int m = n/2;
	char* half_pow = intal_pow(intal1,m);
	char* temp = intal_multiply(half_pow,half_pow);
	free(half_pow);
	
	if(n%2)
	{
		char* result = intal_multiply(temp,intal1);
		free(temp);
		return result;
	}
	
	return temp;
}


// Returns Greatest Common Devisor of intal1 and intal2.
// Let GCD be "0" if both intal1 and intal2 are "0" even though it is undefined, mathematically.
// Euclid's algorithm for GCD has been implemented, where modulus operation is used instead of sutraction to find the GCD of given intals.
char* intal_gcd(const char* intal1, const char* intal2)
{
	int cmp = intal_compare(intal1,intal2);
	if(cmp == 0)
	{
		char* result = malloc((strlen(intal1)+1)*sizeof(char));
		memcpy(result,intal1,(strlen(intal1)+1) * sizeof(char));
		
		return result;
	}
	
	char* intal3 = NULL;
	
	int sec_cmp = intal_compare(intal2,"0");
	if(sec_cmp == 0)
	{
		char* result = malloc((strlen(intal1)+1)*sizeof(char));
		memcpy(result,intal1,(strlen(intal1)+1) * sizeof(char));
		return result;
	}
	
	intal3 = intal_mod(intal1,intal2);
	char* result = intal_gcd(intal2,intal3);
	
	if(intal3) free(intal3);
	return result;
}

// Returns nth fibonacci number.
// intal_fibonacci(0) = intal "0".
// intal_fibonacci(1) = intal "1".
// Fibonacci table is constructed to store the intermediate values.
// Once the intermediate values are calculated, nth fibonacci number can be calculated by adding the last 2 fibonacci numbers in the series.
static char* find_fibonacci(unsigned int n, char** fib)
{
	if(fib[n] != NULL) return fib[n];
	
	char* a = find_fibonacci(n-2,fib);
	fib[n-2] = a;
	
	char* b = find_fibonacci(n-1,fib);
	fib[n-1] = b;
	
	char* result = intal_add(a,b);	
	return result;
}

char* intal_fibonacci(unsigned int n)
{
	char** fib = malloc((n+1)*sizeof(char*));
	for(int i = 2; i<n+1 ; ++i)	fib[i] = NULL;
	
	fib[0] = malloc(2*sizeof(char));
	fib[0][0] = '0';
	fib[0][1] = '\0';
	
	fib[1] = malloc(2*sizeof(char));
	fib[1][0] = '1';
	fib[1][1] = '\0';
	
	
	char* result = find_fibonacci(n,fib);
	
	for(int i = 0; i<n ; ++i) free(fib[i]);
	free(fib);
	
	return result;	
}


// Returns the factorial of n.
// Numbers starting from 1 to n are multipled in an iterative approach and returned.	
char* intal_factorial(unsigned int n)
{
	char one[] = "1";
	
	char* num = malloc(2*sizeof(char));
	num[0] = '1';
	num[1] = '\0';
	
	char* result = num;
	char* next = NULL;
	
	for(int i = 1; i<n; ++i)
	{
		next = intal_add(num,one);
		result = intal_multiply(result,next);
		
		free(num);
		num = next;		
	}
	
	if(next) free(next);
	return result;	
}

// Returns the Binomial Coefficient C(n,k).
// 0 <= k <= n
// C(n,k) < 10^1000 because the returning value is expected to be less than 10^1000.
// Use the Pascal's identity C(n,k) = C(n-1,k) + C(n-1,k-1)
// Make sure the intermediate intal values do not cross C(n,k).
// Use Dynamic Programming. Use extra space of not more than O(k) number of intals. Do not allocate the whole O(nk) DP table.
// Don't let C(1000,900) take more time than C(1000,500). Time limit may exceed otherwise.
char* intal_bincoeff(unsigned int n, unsigned int k)
{
	k = (k<(n-k))?k:(n-k);
	char** table = malloc((k+1)	* sizeof(char*));
	
	table[0] = malloc(2*sizeof(char));
	table[0][0] = '1';
	table[0][1] = '\0';
	for(int i = 1;i<k+1;++i)
	{
		table[i] = malloc(2*sizeof(char));
		table[i][0] = '0';
		table[i][1] = '\0';
	}
	
	for(int i = 1; i<=n; ++i)
	{
		
		int j = (i<k)?i:k;
		while(j>0)
		{
			char* tmp = table[j];
			table[j] = intal_add(tmp,table[j-1]);
			free(tmp);
			--j;
		}
	}
	
	char* result = table[k];
	for(int i = 0; i<k; ++i)	free(table[i]);
	free(table);
	
	return result;
}

// Returns the offset of the largest intal in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000
int intal_max(char **arr, int n)
{
	int max = 0;
	for(int i = 1; i<n; ++i)
	{
		if(intal_compare(arr[i],arr[max]) == 1) max = i;
	}
	return max;
}
// Returns the offset of the smallest intal in the array.
// Return the smallest offset if there are multiple occurrences.
// 1 <= n <= 1000
int intal_min(char **arr, int n)
{
	int min = 0;
	for(int i = 1; i<n; ++i)
	{
		if(intal_compare(arr[i],arr[min]) == -1) min = i;
	}
	return min;
}

// Returns the offset of the first occurrence of the key intal in the array.
// Returns -1 if the key is not found.
// 1 <= n <= 1000
int intal_search(char **arr, int n, const char* key)
{
	int index = -1;
	for(int i = 0; i<n; ++i)
	{
		if(intal_compare(arr[i],key) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
}

// Returns the offset of the first occurrence of the key intal in the SORTED array.
// Returns -1 if the key is not found.
// The array is sorted in nondecreasing order.
// 1 <= n <= 1000
// The implementation should be a O(log n) algorithm.
int intal_binsearch(char **arr, int n, const char* key)
{
	int comp;
	
	int low = 0;
	int high = n;
	int mid = (low+high)/2;
	
	
	while(low<high)
	{	
		mid = (low+high)/2;
		comp = intal_compare(key,arr[mid]);
		if(comp <= 0)
			high = mid;
		else
			low = mid+1;
	}
	if(intal_compare(key,arr[high]) == 0) return high;
	return -1;
	
}

// Sorts the array of n intals.
// 1 <= n <= 1000
// The implementation should be a O(n log n) algorithm.
void intal_sort(char **arr, int n)
{
	intal_merge_sort(arr,0,n-1);
}

static void intal_merge_sort(char** arr,int start, int end)
{
	if(start < end)
	{
		int mid = (start+end)/2;
		intal_merge_sort(arr,start,mid);
		intal_merge_sort(arr,mid+1,end);
		
		intal_merge(arr,start,mid,end);
	}
}

static void intal_merge(char** arr,int start, int mid, int end)
{
	int len = end - start + 1;
	char* temp[len];
	
	int i = start;
	int j = mid + 1;
	int k = 0;
	
	while((i <= mid) && (j <= end))
	{
		int cmp = intal_compare(arr[i],arr[j]);
		if(cmp == -1)
		{
			temp[k] = arr[i];
			++i;
		}
		else
		{
			temp[k] = arr[j];
			++j;
		}
		++k;
	}
	
	while(i<=mid)	temp[k++] = arr[i++];
	while(j<=end)	temp[k++] = arr[j++];
	
	memcpy(&arr[start],temp,len * sizeof(char*));
}

// Coin-Row Problem - Dynamic Programming Solution
// There is a row of n coins whose values are some positive integers C[0..n-1].
// The goal is to pick up the maximum amount of money subject to the constraint that
// no two coins adjacent in the initial row can be picked up.
// 1 <= n <= 1000
// The implementation should be O(n) time and O(1) extra space even though the DP table may be of O(n) size.
// Eg: Coins = [10, 2, 4, 6, 3, 9, 5] returns 25
char* coin_row_problem(char **arr, int n)
{
    char *excl = malloc(2*sizeof(char));
    excl[0] = '0';
    excl[1] = '\0';
    
    char* incl = malloc((strlen(arr[0])+1)*sizeof(char));
    memcpy(incl,arr[0],(strlen(arr[0])+1)*sizeof(char));
    
    char* incl_new = NULL;
    
    for(int i = 1;i<n;i++)
    {
        incl_new = intal_add(arr[i],excl);
        free(excl);
        
        if(intal_compare(incl_new,incl) == -1)
        {
            free(incl_new);
            excl = malloc((strlen(incl)+1)*sizeof(char));
            memcpy(excl,incl,(strlen(incl)+1)*sizeof(char));
        }
        else
        {
            excl = incl;
            incl = incl_new;
        }
    }
    
    char* result = incl;
    free(excl);
    return result;
}

