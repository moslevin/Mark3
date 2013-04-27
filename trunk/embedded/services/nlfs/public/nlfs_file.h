#ifndef __NLFS_FILE_H
#define __NLFS_FILE_H

#include "kerneltypes.h"
#include "nlfs.h"

typedef enum
{
    NLFS_FILE_CREATE,       //!< Create the file if it does not exist
    NLFS_FILE_APPEND,       //!< Open to end of file
    NLFS_FILE_TRUNCATE,     //!< Truncate file size to 0-bytes
//---
    NLFS_FILE_MODES
} NLFS_File_Mode_t;

/*!
 * \brief The NLFS_File class
 */
class NLFS_File
{

public:
    /*!
     * \brief Open
     * \param pclFS_
     * \param szPath_
     * \param eMode_
     * \return
     */
    int     Open(NLFS *pclFS_, const K_CHAR *szPath_, NLFS_File_Mode_t eMode_);

    /*!
     * \brief Read
     * \param ulLen_
     * \param pvBuf_
     * \return
     */
    int     Read(void *pvBuf_, K_ULONG ulLen_);

    /*!
     * \brief Write
     * \param ulLen_
     * \param pvBuf_
     * \return
     */
    int     Write(void *pvBuf_, K_ULONG ulLen_);


    int     Seek(K_ULONG ulOffset_);

    /*!
     * \brief Close
     * \return
     */
    int     Close(void);

private:
    NLFS        *m_pclFileSystem;
    K_ULONG     m_ulOffset;
    K_ULONG     m_ulCurrentBlock;
    K_USHORT    m_usFile;
    NLFS_Node_t m_stNode;
};

#endif // __NLFS_FILE_H
