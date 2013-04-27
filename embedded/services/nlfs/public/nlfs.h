/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2013 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file   nlfs.h
    \brief  Nice Little Filesystem (NLFS) implementation for Mark3
*/

#ifndef __NLFS_H__
#define __NLFS_H__

#include "kerneltypes.h"

class NLFS_File;

//---------------------------------------------------------------------------
/*!
    Permission bit definitions
*/
#define PERM_UX     (0x0001)
#define PERM_UW     (0x0002)
#define PERM_UR     (0x0004)
#define PERM_U_ALL  ( PERM_UX | PERM_UW | PERM_UR )

#define PERM_GX     (0x0008)
#define PERM_GW     (0x0010)
#define PERM_GR     (0x0020)
#define PERM_G_ALL  ( PERM_GX | PERM_GW | PERM_GR )

#define PERM_OX     (0x0040)
#define PERM_OW     (0x0080)
#define PERM_OR     (0x0100)
#define PERM_O_ALL  ( PERM_OX | PERM_OW | PERM_OR )

//---------------------------------------------------------------------------
#define INVALID_BLOCK   (0xFFFFFFFF)
#define INVALID_NODE    (0xFFFF)

//---------------------------------------------------------------------------
#define FILE_NAME_LENGTH    (16)

#define FS_CONFIG_BLOCK     (0)
#define FS_ROOT_BLOCK       (1)

//---------------------------------------------------------------------------
/*!
    Enumeration describing the various types of filesystem nodes
    used by NLFS.  A fileysstem node is a fixed-sized data structure
    consisting of a type specifier, and a union of the data structures
    representing each possible block type.
*/
typedef enum
{
    NLFS_NODE_FREE,     //!< File node is free
    NLFS_NODE_ROOT,     //!< Root filesystem descriptor
    NLFS_NODE_FILE,     //!< File node
    NLFS_NODE_DIR,      //!< Directory node
// --
    FILE_BLOCK_COUNTS
} NLFS_Type_t;

//---------------------------------------------------------------------------
/*!
    Data structure for the "file" FS-node type.  Note that this is the same
    as for a directory node (although fewer fields are used for that case,
    as documented).
*/
typedef struct
{
    K_CHAR      acFileName[16];     //!< Arbitrary, 16-char filename

    K_USHORT    usNextPeer;         //!< Index of the next peer file node
    K_USHORT    usPrevPeer;         //!< Index of the previous peer node

    K_UCHAR     ucGroup;            //!< Group ID of the owner
    K_UCHAR     ucUser;             //!< User ID of the owner
    K_USHORT    usPerms;            //!< File permissions (POSIX-style)

    K_USHORT    usParent;           //!< Index of the parent file node
    K_USHORT    usChild;            //!< Index of the first child node

//-- File-specific
    K_ULONG     ulAllocSize;        //!< Size of the file (allocated)
    K_ULONG     ulFileSize;         //!< Size of the file (in-bytes)

    K_ULONG     ulFirstBlock;       //!< Index of the first file block
    K_ULONG     ulLastBlock;        //!< Index of the last file block
} NLFS_File_Node_t;

//---------------------------------------------------------------------------
/*!
    Data structure for the Root-configuration FS-node type
*/
typedef struct
{
    K_USHORT    usNumFiles;         //!< Number of file nodes in the FS
    K_USHORT    usNumFilesFree;     //!< Number of free file nodes
    K_USHORT    usNextFreeNode;     //!< Index of the next free file

    K_ULONG     ulNumBlocks;        //!< Number of blocks in the FS
    K_ULONG     ulNumBlocksFree;    //!< Number of free blocks
    K_ULONG     ulNextFreeBlock;    //!< Index of the next free block

    K_ULONG     ulBlockSize;        //!< Size of each block on disk
    K_ULONG     ulBlockOffset;      //!< Byte-offset to the first block struct
    K_ULONG     ulDataOffset;       //!< Byte-offset to the first data block
} NLFS_Root_Node_t;

//---------------------------------------------------------------------------
/*!
    Filesystem node data structure.  Contains the block type, as well as
    the union between the various FS-node data structures.  This is also
    the same data format as how data is stored "on-disk"
*/
typedef struct
{
    NLFS_Type_t     eBlockType;                 //!< Block type ID

    union   // Depending on the block type, we use one of the following
    {
        NLFS_Root_Node_t        stRootNode;     //!< Root Filesystem Node
        NLFS_File_Node_t        stFileNode;     //!< File/Directory Node
    };
} NLFS_Node_t;

//---------------------------------------------------------------------------
/*!
    Block data structure.  Contains the block index of the next data
    block (either in the file, or in the free-data pool), as well as any
    special flags.
*/
typedef struct
{
    K_ULONG     ulNextBlock;                //!< Index of the next block
    union
    {
        K_UCHAR     ucFlags;                //!< Block Flags
        struct
        {
            unsigned int    uAllocated;     //!< 1 if allocated
            unsigned int    uCheckBit;      //!< Used for continuity checks
        };
    };
} NLFS_Block_t;

//---------------------------------------------------------------------------
/*!
 * \brief Nice Little File System class
 */
class NLFS
{
friend class NLFS_File;
public:

    /*!
     * \brief Format/Create a new filesystem with the configuration specified
     *        in the parameters
     *
     * \param [in] pvHost_      - Pointer to the FS storage object
     *
     * \param [in] ulTotalSize_ - Total size of the object to format (in bytes)
     *
     * \param [in] usNumFiles_  - Number of file nodes to create in the FS.  This
     *                            parameter determines the maximum number of files
     *                            and directories that can exist simultaneously in
     *                            the filesystem.  All filesystem storage not
     *                            allocated towards file nodes is automatically
     *                            used as data-blocks.
     *
     * \param usDataBlockSize_  - Size of each data block (in bytes).  Setting a
     *                            lower block size is a good way to avoid wasting
     *                            space in small-files due to over-allocation of
     *                            storage (size on-disk vs. actual file size).
     *                            However, each block requires a metadata object,
     *                            which can also add to overhead.  Also, file
     *                            read/write speed can vary significantly based
     *                            on the block size - in many scenarios, larger
     *                            blocks can lead to higher throughput.
     */
    void Format(void *pvHost_, K_ULONG ulTotalSize_, K_USHORT usNumFiles_, K_USHORT usDataBlockSize_);

    /*!
     * \brief Re-mount a previously-cerated filesystem using this FS object.
     *
     * \param [in] pvHost_ - Pointer to the filesystem object
     */
    void Mount(void *pvHost_);

    /*!
     * \brief Create_File creates a new file object at the specified path
     * \param [in] szPath_ - Path to the file to create
     * \return ID of the created file, or INVALID_NODE if the path cannot be
     *         resolved, or the file already exists.
     */
    K_USHORT Create_File(const K_CHAR *szPath_);

    /*!
     * \brief Create_Dir creates a new directory at the specified path
     * \param [in] szPath_ - Path to the directory to create
     * \return ID of the created dir, or INVALID_NODE if the path cannot be
     *         resolved, or the file already exists.
     */
    K_USHORT Create_Dir(const K_CHAR *szPath_);

    /*!
     * \brief Find_Parent_Dir returns the directory under which the specified
     *        file object lives
     * \param [in] szPath_ - Path of the file to find parent directory node for
     * \return directory node ID, or INVALID_NODE if the path is invalid.
     */
    K_USHORT Find_Parent_Dir(const K_CHAR *szPath_);

    /*!
     * \brief Find_File returns the file node ID of the object at a given path
     * \param [in] szPath_ - Path of the file to search for
     * \return file node ID, or INVALID_NODE if the path is invalid.
     */
    K_USHORT Find_File(const K_CHAR *szPath_);

    /*!
     * \brief Print displays a summary of files in the filesystem
     */
    void Print(void);

    /*!
     * \brief GetBlockSize retrieves the data block size for the filesystem.
     * \return The size of a data block in the filesystem, as configured at format.
     */
    K_ULONG GetBlockSize(void) { return m_stLocalRoot.ulBlockSize; }
protected:

    /*!
     * \brief Find_Last_Slash Finds the location of the last '/' character in a
     *        path.
     * \param [in] szPath_ - String representing a '/' delimited path.
     * \return the byte offset of the last slash char in the path.
     */
    K_CHAR Find_Last_Slash(const K_CHAR *szPath_);

    /*!
     * \brief File_Names_Match Determines if a given path matches the name in a
     *        file node.
     * \param [in] szPath_ - file path to search for
     * \param [in] pstNode_ - pointer to a fs node
     * \return true if the filename in the path matches the filename in the node.
     */
    K_BOOL File_Names_Match(const K_CHAR *szPath_, NLFS_Node_t *pstNode_);

    /*!
     * \brief Read_Node is an implementation-specific method used to read a
     *        file node from physical storage into a local data struture.
     * \param [in] usNode_ - File node index
     * \param [out] pstNode_ - Pointer to the file node object to read into
     */
    virtual void Read_Node(K_USHORT usNode_, NLFS_Node_t *pstNode_) = 0;

    /*!
     * \brief Write_Node is an implementation-specific method used to write a
     *        file node from a local structure back to the physical storage.
     * \param [in] usNode_ - File node index
     * \param [in] pstNode_ - Pointer to the file node object to write from
     */
    virtual void Write_Node(K_USHORT usNode_, NLFS_Node_t *pstNode_) = 0;

    /*!
     * \brief Read_Block_Header is an implementation-specific method used to read
     *        a file block header from physical storage into a local struct.
     * \param [in] ulBlock_ - data block index
     * \param [out] pstBlock_ - block header structure to read into
     */
    virtual void Read_Block_Header(K_ULONG ulBlock_, NLFS_Block_t *pstBlock_) = 0;

    /*!
     * \brief Write_Block_Header is an implementation-specific method used to write
     *        a file block header back to physical storage from a local struct
     * \param [in] ulBlock_ - data block index
     * \param [in] pstNode_ - pointer to the local data structure to write from
     */
    virtual void Write_Block_Header(K_ULONG ulBlock_, NLFS_Block_t *pstFileBlock_) = 0;

    /*!
     * \brief Read_Block is an implementation-specific method used to read raw file
     *        data from physical storage into a local buffer.
     *
     * \param [in] ulBlock_ - filesystem block ID corresponding to the file
     * \param [in] ulOffset_ - offset (in bytes) from the beginning of the block
     * \param [out] pvData_ - output buffer to read into
     * \param [in] ulLen_ - length of data to read (in bytes)
     */
    virtual void Read_Block(K_ULONG ulBlock_, K_ULONG ulOffset_, void *pvData_, K_ULONG ulLen_) = 0;

    /*!
     * \brief Write_Block is an implementation-specific method used to write a
     *        piece of file data to its data block in the underlying physical
     *        storage.
     *
     * \param [in] ulBlock_ - filesystem block ID corresponding to the file
     * \param [in] ulOffset_ - offset (in bytes) from the beginning of the block
     * \param [in] pvData_ - data buffer to write to disk
     * \param [in] ulLen_ - length of data to write (in bytes)
     */
    virtual void Write_Block(K_ULONG ulBlock_, K_ULONG ulOffset_, void *pvData_, K_ULONG ulLen_) = 0;

    /*!
     * \brief Print_File_Details displays information about a given file node
     * \param [in] usNode_  - file index to display details for
     */
    void Print_File_Details(K_USHORT usNode_);

    /*!
     * \brief Print_Dir_Details displays information about a given directory node
     * \param [in] usNode_ - directory index to display details for
     */
    void Print_Dir_Details(K_USHORT usNode_);

    /*!
     * \brief Print_Node_Details prints details about a node, the details differ
     *        based on whether it's a file/directory/root node.
     * \param [in] usNode_ - node to show details for
     */
    void Print_Node_Details(K_USHORT usNode_);

    /*!
     * \brief Push_Free_Node returns a file node back to the free node list
     * \param [in] usNode_  - index of the file node to push back to the free list.
     */
    void Push_Free_Node(K_USHORT usNode_);

    /*!
     * \brief Pop_Free_Node returns the first free file node in the free list
     * \return the index of the file node popped off the free list
     */
    K_USHORT Pop_Free_Node(void);

    /*!
     * \brief Push_Free_Block returns a file block back to the head of the free
     *        block list
     * \param [in] ulBlock_ - index of the data block to free
     */
    void Push_Free_Block(K_ULONG ulBlock_);

    /*!
     * \brief Pop_Free_Block pops a file data block from the head of the free list
     * \return the block index of the file node popped from the head of the free
     *         block list
     */
    K_ULONG Pop_Free_Block(void);

    /*!
     * \brief Append_Block_To_Node adds a file data block to the end of a file
     * \param [in] pstFile_ - Pointer to the file node to add a block to
     * \return Data block ID of the allocated block, or INVALID_BLOCK on failure.
     */
    K_ULONG Append_Block_To_Node(NLFS_Node_t *pstFile_);

    /*!
     * \brief Create_File_i is the private method used to create a file or directory
     * \param [in] szPath_ - Path of the file or directory to create
     * \param [in] eType_ - Type of file to create
     * \return File node ID of the newly created file, or INVALID_NODE on failure.
     */
    K_USHORT Create_File_i(const K_CHAR *szPath_, NLFS_Type_t eType_);

    /*!
     * \brief Set_Node_Name sets the name of a file or directory node
     * \param [in] pstFileNode_ - Pointer to a file node structure to name
     * \param [in] szPath_ - Name for the file
     */
    void Set_Node_Name( NLFS_Node_t *pstFileNode_, const K_CHAR *szPath_ );

    void *m_pvHost;                         //!< Local, cached copy of user pointer
    NLFS_Root_Node_t m_stLocalRoot;         //!< Local, cached copy of root
};

#endif
