#if !defined(__RingBuffer_hdr__)
#define __RingBuffer_hdr__

#include <cassert>
#include <algorithm>
#include <cmath>

/*! \brief implement a circular buffer of type T
 */
template <class T>
class CRingBuffer
{
public:
    explicit CRingBuffer (int iBufferLengthInSamples) :
    m_iBuffLength(iBufferLengthInSamples),
    m_iReadIdx(0),
    m_iWriteIdx(0),
    m_ptBuff(0)
    {
        assert(iBufferLengthInSamples > 0);
        
        m_ptBuff        = new T [m_iBuffLength];
        reset();
    }
    
    virtual ~CRingBuffer ()
    {
        delete [] m_ptBuff;
        m_ptBuff    = 0;
    }
    
    /*! add a new value of type T to write index
     \param tNewValue the new value
     \return void
     */
    void put(T tNewValue)
    {
        m_ptBuff[m_iWriteIdx] = tNewValue;
    }
    
    /*! return the value at the current read index
     \return type T the value from the read index
     */
    T get () const
    {
        return m_ptBuff[m_iReadIdx];
    }
    
    /*! return the current index for writing/put
     \return int
     */
    int getWriteIdx () const
    {
        return m_iWriteIdx;
    }
    
    /*! move the write index to a new position
     \param iNewWriteIdx: new position
     \return void
     */
    void setWriteIdx (int iNewWriteIdx)
    {
        try {
            checkIndex(iNewWriteIdx);
        }
        catch (char const * msg) {
            std::cout << msg << std::endl;
            return;
        }
        
        m_iWriteIdx = iNewWriteIdx;
    }
    
    /*! return the current index for reading/get
     \return int
     */
    int getReadIdx () const
    {
        return m_iReadIdx;
    }
    
    /*! move the read index to a new position
     \param iNewReadIdx: new position
     \return void
     */
    void setReadIdx (int iNewReadIdx)
    {
        try {
            checkIndex(iNewReadIdx);
        }
        catch (char const* msg) {
            std::cout << msg << std::endl;
            return;
        }
        m_iReadIdx = iNewReadIdx;
    }
    
    /*! add a new value of type T to write index and increment write index
     \param tNewValue the new value
     \return void
     */
    void putPostInc (T tNewValue)
    {
        // to be implemented
        put(tNewValue);
        if (m_iWriteIdx == m_iReadIdx && status == 1) {
            incIndex(m_iReadIdx);
            std::cout << "WARNING: you have override the value not been read yet" << std::endl;
        }
        incIndex(m_iWriteIdx);
        // check if write index and read index overlap
        if (m_iWriteIdx == m_iReadIdx && status == 0) {
            status = 1;
            std::cout << "WARNING: you are going to override the value that you have not read" << std::endl;
        }
    }
    
    /*! return the value at the current read index and increment the read pointer
     \return type T the value from the read index
     */
    T getPostInc ()
    {
        // to be implemented
        T temp = get();
        if (m_iWriteIdx == m_iReadIdx && status == 0) {
            incIndex(m_iWriteIdx);
            std::cout << "you have read a default value or one already been read before " << std::endl;
        }
        incIndex(m_iReadIdx);
        // check if write index and read index overlap
        if (m_iWriteIdx == m_iReadIdx && status == 1) {
            status = 0;
            std::cout << "WARNING: the value you are going "
            "to read is either the value you have already read or a value not defined" << std::endl;
        }
        return temp;
    }
    
    /*! return the value at the index with an arbitrary offset
     \param iOffset: read at offset from read index
     \return type T the value from the read index
     */
    T get (int iOffset) const
    {
        try {
            checkIndex(iOffset);
        }
        catch (char const* msg) {
            std::cout << msg << std::endl;
//            return;
        }
        
        int index = (m_iReadIdx + iOffset) % m_iBuffLength;
        float  a = m_ptBuff[index];
        return a;
    }
    
    /*! set buffer content and indices to 0
     \return void
     */
    void reset ()
    {
        memset (m_ptBuff, 0, sizeof(T)*m_iBuffLength);
        m_iReadIdx  = 0;
        m_iWriteIdx = 0;
    }
    
    /*! returns the number of values currently buffered (note: 0 could also mean the buffer is full!)
     \return int
     */
    int getNumValuesInBuffer () const
    {
        return (m_iWriteIdx - m_iReadIdx + m_iBuffLength)%m_iBuffLength;
    }
    
    /*! returns the length of the internal buffer
     \return int
     */
    int getLength () const
    {
        return m_iBuffLength;
    }
    int getStatus() {
        return status;
    }
private:
    CRingBuffer ();
    CRingBuffer(const CRingBuffer& that);
    
    int m_iBuffLength,              //!< length of the internal buffer
    m_iReadIdx,                 //!< current read index
    m_iWriteIdx;                //!< current write index
    
    T   *m_ptBuff;                  //!< data buffer
    void checkIndex(int newIndex) const{
        if (newIndex < 0 || newIndex >= m_iBuffLength ) {
            throw "Exception: index out of range!";
        }
    }

    void incIndex(int &index) {
        if (index == m_iBuffLength - 1) {
            index = 0;
        } else {
            ++index;
        }
    }
    int status = 0;                 // if status = 0 && read index = write index, put
    // if status = 1 && read index = write index, read
};
#endif // __RingBuffer_hdr__

