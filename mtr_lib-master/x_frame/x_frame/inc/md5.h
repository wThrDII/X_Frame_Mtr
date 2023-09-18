#ifndef MD5_H
#define MD5_H
 
 // 参考资料: https://blog.csdn.net/Oliver_xpl/article/details/90214896
typedef struct
{
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];   
}MD5_CTX;
 
 /***********************************
* 非线性函数
* (&是与,|是或,~是非,^是异或) 
* 
* 这些函数是这样设计的：
*   如果X、Y和Z的对应位是独立和均匀的，
*   那么结果的每一位也应是独立和均匀的。 
* 
* 函数F是按逐位方式操作：如果X，那么Y，否则Z。
* 函数H是逐位奇偶操作符
**********************************/
#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))

/**************************************
*向左移(右移)n个单位
* ************************************/
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))

/****************************************************
* 每次操作对a，b，c和d中的其中三个作一次非线性函数运算
*  F(b,c,d)   G(b,c,d)   H(b,c,d)   I(b,c,d)
*
* 然后将所得结果加上 第四个变量(a)，
* F(b,c,d)+a
*
* 文本的一个子分组(x)
* F(b,c,d)+a+x
* 
* 和一个常数(ac)。
* F(b,c,d)+a+x+ac
*
* 再将所得结果向右环移一个不定的数(s)，
* ROTATE_LEFT( F(b,c,d)+a+x+ac , s )
* 
* 并加上a，b，c或d中之一(b)。
* ROTATE_LEFT( F(b,c,d)+a+x+ac , s )+b
* 
* 最后用该结果取代a，b，c或d中之一(a)。
* a=ROTATE_LEFT( F(b,c,d)+a+x+ac , s )+b
* 
* ***************************************************/
#define FF(a,b,c,d,x,s,ac) \
          { \
          a += F(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define GG(a,b,c,d,x,s,ac) \
          { \
          a += G(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define HH(a,b,c,d,x,s,ac) \
          { \
          a += H(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define II(a,b,c,d,x,s,ac) \
          { \
          a += I(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
/**
 * @brief  初始化MD5 test
 *
 * @param [in] context MD5结构体指针
 *
 * @details:
 * @par e.g:
 * @code
 *      MD5_CTX user_md5;  //定义一个MD5 test
 *
 *      MD5Init(user_md5);
 * @endcode
 */
void MD5Init(MD5_CTX *context);

/**
 * @brief 对一个MD5 test, 把输入的数据进行分组，并进行加密
 * 未用到的数据存储在MD5 test中
 *
 * @param [in] context MD5 test
 * @param [in] input 新加入的数据
 * @param [in] inputlen 新加入数据的长度(字节)
 *
 * @details:
 * @par e.g:
 * @code
 *      unsigned char encrypt[1000] = {"user data"};//要加密内容
 *
 *      MD5Update(&md5, encrypt, strlen((char *)encrypt));
 * @endcode
 */
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);

/**
 * @brief 对数据进行补足，并加入数据位数信息，并进一步加密
 * 
 * @param [in] context MD5 test
 * @param [in] digest        
 * 
 * @details: 
 * @par e.g:
 * @code
 *      unsigned char decrypt[16]; //加密结果
 * 
 *      MD5Final(&md5,decrypt);
 * @endcode
 */
void MD5Final(MD5_CTX *context,unsigned char digest[16]);

/**
 * @brief 对512位的block数据进行加密，并把加密结果存入state数组中
 *  对512位信息(即block字符数组)进行一次处理，每次处理包括四轮
 *
 * @param [in] state[4] md5结构中的state[4]，用于保存对512bits信息加密的中间结果或者最终结果
 * @param [in] block 欲加密的512bits信息或其中间数据
 *
 * @details:
 * @par e.g:
 * @code
 *      MD5Transform(context->state, context->buffer);  //context是MD5_CTX
 * @endcode
 */
void MD5Transform(unsigned int state[4],unsigned char block[64]);

/**
 * @brief 利用位操作，按1->4方式把数字分解成字符
 * 
 * @param [in] output 输出的字符的数组        
 * @param [in] input 输入数字的数组        
 * @param [in] len 输入数字数组的长度（单位：位）          
 * 
 * @details: 
 * @par e.g:
 * @code
 *      unsigned char bits[8];
 *      
 *      MD5Encode(bits,context->count,8);  //context是MD5_CTX
 * @endcode
 */
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);

/**
 * @brief 利用位操作，按4->1方式把字符合成数字
 * 
 * @param [in] output 利用位操作，按4->1方式把字符合成数字        
 * @param [in] input 输入字符的数组        
 * @param [in] len 输入字符的长度 （单位：位）           
 * 
 * @details: 
 * @par e.g:
 * @code
 *      unsigned int x[64]; 
 * 
 *      MD5Decode(x,block,64);
 * @endcode
 */
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);
 
#endif
