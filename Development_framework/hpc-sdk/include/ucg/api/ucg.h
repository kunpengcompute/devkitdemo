/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#ifndef UCG_H_
#define UCG_H_

#include <ucg/api/ucg_version.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup UCG_BASE UCG Base
 * @{
 * Base data structures and routines used by other groups.
 * @}
 */

/**
 * @defgroup UCG_DT UCG Data Type
 * @{
 * Data structures and routines of UCG Data Type.
 * @}
 */

/**
 * @defgroup UCG_CONTEXT UCG Application Context
 * @{
 * Data structures and routines of UCG Application Context.
 * @}
 */

/**
 * @defgroup UCG_GLOBAL UCG GLOBAL RESOURCES
 * @{
 * Data structures and routines of UCG GLOBAL RESOURCES.
 * @}
 */

/**
 * @defgroup UCG_OOB UCG OOB RESOURCES
 * @{
 * Data structures and routines of UCG OOB RESOURCES.
 * @}
 */

/**
 * @defgroup UCG_GROUP UCG Communication Group
 * @{
 * Data structures and routines of UCG Communication Group.
 * @}
 */

/**
 * @defgroup UCG_REQUEST UCG Collective Operation Request
 * @{
 * Data structures and routines of UCG Collective Operation Request.
 * @}
 */


/*
 *******************************************************************************
 *                            Macro define
 *******************************************************************************
 */
/**
 * @ingroup UCG_BASE
 * @brief The i-th bit.
 */
#define UCG_BIT(i) (1ul << (i))

/**
 * @ingroup UCG_BASE
 * @brief Mask of bits 0..i-1.
 */
#define UCG_MASK(i) (UCG_BIT(i) - 1)

/**
 * @ingroup UCG_BASE
 * @brief If passing UCG_IN_PLACE as the value of sendbuf, UCG will treat the
 * recv buffer as send buffer. In such a case, sendcount and sendtype are ignored.
 */
#define UCG_IN_PLACE ((void*)1)

/**
 * @ingroup UCG_BASE
 * @brief Common invalid rank.
 */
#define UCG_INVALID_RANK ((ucg_rank_t)-1)


/*
 *******************************************************************************
 *                            Type define
 *******************************************************************************
 */

/**
 * @ingroup UCG_BASE
 * @brief Process identifier.
 *
 * If the size of group is N. The range of rank is [0, N).
 */
typedef int32_t ucg_rank_t;

/**
 * @ingroup UCG_DT
 * @brief UCG datatype handle.
 */
typedef struct ucg_dt* ucg_dt_h;

/**
 * @ingroup UCG_DT
 * @brief UCG reduction operation handle.
 */
typedef struct ucg_op* ucg_op_h;

/**
 * @ingroup UCG_CONTEXT
 * @brief UCG configuration descriptor.
 */
typedef struct ucg_config* ucg_config_h;

/**
 * @ingroup UCG_CONTEXT
 * @brief UCG context descriptor.
 */
typedef struct ucg_context* ucg_context_h;

/**
 * @ingroup UCG_GROUP
 * @brief UCG group descriptor.
 */
typedef struct ucg_group* ucg_group_h;

/**
 * @ingroup UCG_REQUEST
 * @brief Collective request descriptor.
 */
typedef struct ucg_request* ucg_request_h;

/**
 * @ingroup UCG_BASE
 * @brief Status codes.
 *
 * Use negative number to indicate failure.
 */
typedef enum {
    /* Operation is successful. */
    UCG_OK = 0,
    /* Operation is still in progress. */
    UCG_INPROGRESS = 1,

    /* Failure codes. */
    UCG_ERR_UNSUPPORTED = -1,
    UCG_ERR_INVALID_PARAM = -2,
    UCG_ERR_NO_RESOURCE = -3,
    UCG_ERR_NOT_FOUND = -4,
    UCG_ERR_NO_MEMORY = -5,
    UCG_ERR_INCOMPATIBLE = -6,
    UCG_ERR_IO_ERROR = -7,
    UCG_ERR_TRUNCATE = -8,
} ucg_status_t;

/**
 * @ingroup UCG_BASE
 * @brief UCG location field mask.
 */
typedef enum {
    UCG_LOCATION_FIELD_SOCKET_ID = UCG_BIT(0),
    UCG_LOCATION_FIELD_NODE_ID = UCG_BIT(1),
    UCG_LOCATION_FIELD_SUBNET_ID = UCG_BIT(2),
} ucg_location_field_t;

/**
 * @ingroup UCG_BASE
 * @brief Rank mapping type.
 */
typedef enum {
    UCG_RANK_MAP_TYPE_FULL, /**< dst_rank = src_rank */
    UCG_RANK_MAP_TYPE_ARRAY, /**< dst_rank = ranks[src_rank] */
    UCG_RANK_MAP_TYPE_STRIDE, /**< dst_rank = start + src_rank * stride */
    UCG_RANK_MAP_TYPE_CB, /**< dst_rank = mapping(arg, src_rank) */
} ucg_rank_map_type_t;

/**
 * @ingroup UCG_BASE
 * @brief Memory type.
 */
typedef enum {
    UCG_MEM_TYPE_HOST, /**< Default system memory. */
    UCG_MEM_TYPE_ACL, /**< Huawei ACL memory. */
    UCG_MEM_TYPE_UNKNOWN, /**< Internal automatic detect. */
    UCG_MEM_TYPE_LAST = UCG_MEM_TYPE_UNKNOWN,
} ucg_mem_type_t;

/**
 * @ingroup UCG_BASE
 * @brief Memory attributes field mask.
 */
typedef enum {
    UCG_MEM_ATTR_FIELD_MEM_TYPE = UCG_BIT(0), /**< Memory type. */
} ucg_mem_attr_field_t;

/**
 * @ingroup UCG_BASE
 * @brief UCG thread mode.
 */
typedef enum {
    UCG_THREAD_MODE_SINGLE, /**< Only one thread invokes ucg routines. */
    UCG_THREAD_MODE_MULTI, /**< Multiple threads invoke UCG routines concurrently. */
    UCG_THREAD_MODE_LAST,
} ucg_thread_mode_t;

/**
 * @ingroup UCG_DT
 * @brief UCG datatype parameters field mask.
 */
typedef enum {
    UCG_DT_PARAMS_FIELD_TYPE = UCG_BIT(0),
    UCG_DT_PARAMS_FIELD_USER_DT = UCG_BIT(1),
    UCG_DT_PARAMS_FIELD_SIZE = UCG_BIT(2),
    UCG_DT_PARAMS_FIELD_EXTENT = UCG_BIT(3),
    UCG_DT_PARAMS_FIELD_CONV = UCG_BIT(4),
    UCG_DT_PARAMS_FIELD_TRUE_LB = UCG_BIT(5),
    UCG_DT_PARAMS_FIELD_TRUE_EXTENT = UCG_BIT(6)
} ucg_dt_params_field_t;

/**
 * @ingroup UCG_DT
 * @brief Data types
 * @note This is a union set of data types supported by all platforms which means
 * some data types are not available with a particular memory type. In the unavailable
 * case, UCG will return UCG_ERR_UNSUPPORTED.
 */
typedef enum {
    /* Intrinsic data types. */
    UCG_DT_TYPE_INT8,
    UCG_DT_TYPE_INT16,
    UCG_DT_TYPE_INT32,
    UCG_DT_TYPE_INT64,
    UCG_DT_TYPE_UINT8,
    UCG_DT_TYPE_UINT16,
    UCG_DT_TYPE_UINT32,
    UCG_DT_TYPE_UINT64,
    UCG_DT_TYPE_FP16,
    UCG_DT_TYPE_FP32,
    UCG_DT_TYPE_FP64,
    UCG_DT_TYPE_PREDEFINED_LAST,

    /* User-defined data type. */
    UCG_DT_TYPE_USER,
} ucg_dt_type_t;

/**
 * @ingroup UCG_DT
 * @brief UCG reduction operation parameters field mask.
 */
typedef enum {
    UCG_OP_PARAMS_FIELD_TYPE = UCG_BIT(0),
    UCG_OP_PARAMS_FIELD_USER_OP = UCG_BIT(1),
    UCG_OP_PARAMS_FIELD_USER_FUNC = UCG_BIT(2),
    UCG_OP_PARAMS_FIELD_COMMUTATIVE = UCG_BIT(3),
} ucg_op_params_field_t;

/**
 * @ingroup UCG_DT
 * @brief Reduction operation types.
 * @note This is a union set of reduction operation types supported by all
 * platforms which means some reduction operation types are not available with
 * a particular data type using a particular memory type. In the unavailable
 * case, UCG will return UCG_ERR_UNSUPPORTED.
 */
typedef enum {
    /* Intrinsic reduction operation. */
    UCG_OP_TYPE_MAX,
    UCG_OP_TYPE_MIN,
    UCG_OP_TYPE_SUM,
    UCG_OP_TYPE_PROD,
    UCG_OP_TYPE_PREDEFINED_LAST,

    /* User-defined reduction operation. */
    UCG_OP_TYPE_USER,
} ucg_op_type_t;

/**
 * @ingroup UCG_CONTEXT
 * @brief UCG context parameters field mask.
 */
typedef enum {
    UCG_PARAMS_FIELD_OOB_GROUP = UCG_BIT(0), /**< Out Of Band communication group */
    UCG_PARAMS_FIELD_LOCATION_CB = UCG_BIT(1), /**< Callback to get location of process */
    UCG_PARAMS_FIELD_THREAD_MODE = UCG_BIT(2), /**< Context thread mode */
} ucg_params_field_t;

/**
 * @ingroup UCG_GROUP
 * @brief UCG group parameters field mask.
 */
typedef enum {
    UCG_GROUP_PARAMS_FIELD_ID = UCG_BIT(0), /**< Group ID */
    UCG_GROUP_PARAMS_FIELD_SIZE = UCG_BIT(1), /**< Group size*/
    UCG_GROUP_PARAMS_FIELD_MYRANK = UCG_BIT(2), /**< My rank in the group */
    UCG_GROUP_PARAMS_FIELD_RANK_MAP = UCG_BIT(3), /**< Rank map */
    UCG_GROUP_PARAMS_FIELD_OOB_GROUP = UCG_BIT(4), /**< Out Of Band communication group */
} ucg_group_params_field_t;

/**
 * @ingroup UCG_GLOBAL
 * @brief UCG global parameters field mask.
 */
typedef enum {
    UCG_GLOBAL_PARAMS_FIELD_OOB_RESOURCE = UCG_BIT(0), /**< oob resources */
} ucg_global_params_field_t;

/**
 * @ingroup UCG_REQUEST
 * @brief UCG request information field mask.
 */
typedef enum {
    UCG_REQUEST_INFO_FIELD_MEM_TYPE = UCG_BIT(0), /**< Memory type. */
    UCG_REQUEST_INFO_FIELD_CB = UCG_BIT(1), /**< Request completion callback. */
} ucg_request_info_field_t;

/**
 * @ingroup UCG_BASE
 * @brief Rank mapping
 */
typedef struct {
    ucg_rank_map_type_t type;
    /* The range of source rank is [0, size). */
    uint32_t size;
    union {
        /* UCG_RANK_MAP_TYPE_ARRAY */
        ucg_rank_t *array;
        /* UCG_RANK_MAP_TYPE_STRIDE */
        struct {
            ucg_rank_t start;
            int32_t stride;
        } strided;
        /* UCG_RANK_MAP_TYPE_CB */
        struct {
            ucg_rank_t (*mapping)(void *arg, ucg_rank_t rank);
            void *arg;
        } cb;
    };
} ucg_rank_map_t;

/**
 * @ingroup UCG_BASE
 * @brief Process's location information.
 */
typedef struct {
    /**
     * Mask of valid fields in this structure, using bits from @ref ucg_location_field_t.
     * Fields not specified in this mask will be ignored.
     * Provides ABI compatibility with respect to adding new fields.
     */
    uint64_t field_mask;

    /**
     * ID of the subnet where the process resides.
     * Should start from 0 to the maximum number of subnet in net.
     */
    int32_t subnet_id;

    /**
     * ID of the node where the process resides.
     * Should start from 0 to the maximum number of nodes.
     */
    int32_t node_id;

    /**
     * ID of the socket where the process resides.
     * Should start from 0 to the maximum number of sockets in node.
     */
    int16_t socket_id;
} ucg_location_t;

/**
 * @ingroup UCG_BASE
 * @brief Memory attributes.
 */
typedef struct {
    /**
     * Mask of the information to be queried,
     * using bits from @ref ucg_mem_attr_field_t
     */
    uint64_t field_mask;

     /** Memory type. Corresponding bit is UCG_MEM_ATTR_FIELD_MEM_TYPE */
    ucg_mem_type_t mem_type;
} ucg_mem_attr_t;

/**
 * @ingroup UCG_DT
 * @brief Convert the memory layout of data.
 */
typedef struct {
    /**
     * @brief Start to pack
     * @param [in] buffer       Buffer to pack
     * @param [in] user_dt      User datatype of elements in buffer
     * @param [in] count        Number of elements in buffer
     * @return Packing state.
     */
    void* (*start_pack)(const void *buffer, void *user_dt, int32_t count);

    /**
     * @brief Pack data to dst buffer.
     * @param [in]     state        Packing state returned by start_pack
     * @param [in]     offset       Offset of the buffer passed to start_pack
     * @param [out]    dst          Destination buffer to save packed data
     * @param [inout]  length       Value-result argument. UCG will initialize it
     *                              to cantain the dst buffer length in bytes; on
     *                              return it need contain the actual packed size
     * @return UCG_OK for success, otherwise error.
     */
    ucg_status_t (*pack)(void *state, uint64_t offset, void *dst, uint64_t *length);

    /**
     * @brief Start to unpack
     * @param [in] buffer       Buffer to be managed by unpacker.
     * @param [in] user_dt      User data type of elements in buffer.
     * @param [in] count        Number of elements in buffer.
     * @return Unpacking state.
     */
    void* (*start_unpack)(void *buffer, void *user_dt, int32_t count);

    /**
     * @brief Unpack data from src buffer.
     * @param [in]     state        Unpacking state returned by start_unpack
     * @param [in]     offset       Offset of the buffer passed to start_unpack
     * @param [out]    src          Source buffer which contains packed data
     * @param [inout]  length       Value-result argument. UCG will initialize it
     *                              to cantain the src buffer length in bytes; on
     *                              return it need contain the actual unpacked size
     * @return UCG_OK for success, otherwise error.
     */
    ucg_status_t (*unpack)(void *state, uint64_t offset, const void *src, uint64_t *length);

    /**
     * @brief Destroy state
     */
    void (*finish)(void *state);
} ucg_dt_convertor_t;

/**
 * @ingroup UCG_DT
 * @brief Parameters of creating UCG data type
 *
 * If type is UCG_DT_TYPE_USER, user needs to fill the following fields
 * - @ref ucg_dt_params_t::user_dt
 * - @ref ucg_dt_params_t::size
 * - @ref ucg_dt_params_t::extent
 *
 * If extent == size is true, user_dt is contiguous. Otherwise non-contiguous.
 * And if user_dt is non-contiguous, the following fields are needed too
 * - @ref ucg_dt_parmas_t::pack
 * - @ref ucg_dt_parmas_t::unpack
 */
typedef struct {
    /**
     * Mask of valid fields in this structure, using bits from @ref ucg_dt_params_field_t.
     * Fields not specified in this mask will be ignored.
     * Provides ABI compatibility with respect to adding new fields.
     */
    uint64_t field_mask;

    ucg_dt_type_t type;
    /** A pointer to user-defined datatype object. */
    void *user_dt;
    /** Size of user-defined datatype if it's put on a contiguous memory. */
    uint64_t size;
    /** Extent of the user-defined datatype. */
    int64_t extent;
    /** User-defined datatype convertor. */
    ucg_dt_convertor_t conv;
    /** true lower bound of the data without user defined lb and ub. */
    int64_t true_lb;
    /** true extent of the data without user defined lb and ub. */
    int64_t true_extent;
} ucg_dt_params_t;

/**
 * @ingroup UCG_DT
 * @brief Reduction operatin function.
 *
 * target[i] = source[i] op target[i].
 */
typedef ucg_status_t (*ucg_op_func_t)(void *op, const void *source,
                                      void *target, int32_t count,
                                      void *dt);

/**
 * @ingroup UCG_DT
 * @brief Parameters of creating reduction operation
 *
 * When type is UCG_OP_TYPE_USER, user needs to fill the following fields
 * - @ref ucg_op_params_t::user_op
 * - @ref ucg_op_params_t::user_func
 * - @ref ucg_op_params_t::commutative
 *
 * @note If reduction operation is non-commutative, the reduction order is
 * determined by the ranks of the processes in the group.
 */
typedef struct {
    /**
     * Mask of valid fields in this structure, using bits from @ref ucg_reduction_op_field_t.
     * Fields not specified in this mask will be ignored.
     * Provides ABI compatibility with respect to adding new fields.
     */
    uint64_t field_mask;

    ucg_op_type_t type;
    /** A pointer to user-defined reduction operation. */
    void *user_op;
    /** The user's function for user-defined reduction operation. */
    ucg_op_func_t user_func;
    /** The user op is commutative or not */
    uint8_t commutative;
} ucg_op_params_t;

/**
 * @ingroup UCG_CONTEXT
 * @brief OOB allgather callback.
 */
typedef ucg_status_t (*ucg_oob_allgather_cb_t)(const void *sendbuf,
                                               void *recvbuf,
                                               int32_t count,
                                               void *group);

/**
 * @ingroup UCG_CONTEXT
 * @brief OOB communication group
 */
typedef struct {
    /**
     * The datatype of elements in the sendbuf and recvbuf can treated as char.
     */
    ucg_oob_allgather_cb_t allgather;

    /** My rank in this group. */
    ucg_rank_t myrank;

    /** Group size */
    uint32_t size;

    /** Number of procs in my node */
    uint32_t num_local_procs;

    /** User-defined oob communication group */
    void *group;
} ucg_oob_group_t;

/**
 * @ingroup UCG_CONTEXT
 * @brief Get location callback.
 */
typedef ucg_status_t (*ucg_get_location_cb_t)(ucg_rank_t rank,
                                              ucg_location_t *location);

/**
 * @ingroup UCG_CONTEXT
 * @brief Tuning parameters for UCG context.
 *
 * The structure defines the parameters that are used for intializing the UCG
 * context.
 */
typedef struct {
    /**
     * Mask of valid fields in this structure, using bits from @ref ucg_params_field_t.
     * Fields not specified in this mask will be ignored.
     * Provides ABI compatibility with respect to adding new fields.
     */
    uint64_t field_mask;

    /**
     * OOB group is used to exchange informations during initializing the UCG
     * context. This group should be consisted of all job processes. For MPI,
     * this can be MPI_COMM_WORLD.
     * This field must be specified. Corresponding bit is UCG_PARAMS_FIELD_OOB_GROUP.
     */
    ucg_oob_group_t oob_group;

    /**
     * Callback to get location of the process with oob_group.rank.
     * If this field is not specified, location will be obtained by UCG itself
     * that is less efficient when the user has obtained the location.
     */
    ucg_get_location_cb_t get_location;

    /**
     * Specifies whether the context is used in a single-thread or multi-thread
     * environment.
     * This is an optional field, default value is UCG_THREAD_MODE_SINGLE.
     * UCG_THREAD_MODE_MULTI is valid when UCG is built with UCG_ENABLE_MT.
     */
    ucg_thread_mode_t thread_mode;
} ucg_params_t;

/**
 * @ingroup UCG_GROUP
 * @brief Tuning parameters for UCG group.
 *
 * The structure defines the parameters that are used for creating the UCG
 * group.
 */
typedef struct {
    /**
     * Mask of valid fields in this structure, using bits from
     * @ref ucg_group_params_field_t.
     * Fields not specified in this mask will be ignored.
     * Provides ABI compatibility with respect to adding new fields.
     */
    uint64_t field_mask;

    /**
     * The field provides a integer identifying the group. The group id must be
     * unique in the job when group is active.
     * This field must be specified. Corresponding bit is UCG_GROUP_PARAMS_FIELD_ID
     */
    uint32_t id;

    /**
     * The field represents the group size.
     * This field must be specified. Corresponding bit is UCG_GROUP_PARAMS_FIELD_SIZE.
     */
    uint32_t size;

    /**
     * The field represents my rank in this group.
     * This field must be specified. Corresponding bit is UCG_GROUP_PARAMS_FIELD_MYRANK
     */
    ucg_rank_t myrank;

    /**
     * The field provides a rank mapping between this group and @ref ucg_params_t.oob_group
     * used by the context.
     * This field must be specified. Corresponding bit is UCG_GROUP_PARAMS_FIELD_RANK_MAP
     */
    ucg_rank_map_t rank_map;

    /**
     * OOB group is used to exchange informations during creating the UCG
     * group. This group should be consisted of the members of UCG group.
     * This field must be specified. Corresponding bit is UCG_GROUP_PARAMS_FIELD_OOB_GROUP.
     */
    ucg_oob_group_t oob_group;
} ucg_group_params_t;

/**
 * @ingroup UCG_OOB
 * @brief Tuning parameters for ucx planc in UCG.
 *
 * The structure defines the parameters used by ucx planc in UCG.
 */
typedef struct {
    /**
     * @brief Callback to get the ucp_ep.
     * @param [in] oob_group     Communication group derived from ucg_group_params_t::oob_group::group
     * @param [in] rank          Rank in communication group derived from oob_group::myrank
     * @return Return ucp_ep. Use void* to hide specific resources.
     */
    void* (*get_ucp_ep)(void *arg, void *oob_group, int rank);

    /**
     * @brief Callback to get the ucp_worker.
     * @return Return ucp_worker. Use void* to hide specific resources.
     */
    void* (*get_ucp_worker)(void *arg);

    /**
     * The user identifies the returned ucp_worker based on the field
     * and the ucp_ep corresponding to the rank of the corresponding ucp_worker.
     */
    void *arg;
} ucg_oob_resource_t;

/**
 * @ingroup UCG_GLOBAL
 * @brief Tuning parameters for UCG.
 *
 * The structure defines the parameters used by UCG when initializing global resources.
 */
typedef struct {
    /**
     * Mask of valid fields in this structure, using bits from
     * @ref ucg_global_params_field_t.
     * Fields not specified in this mask will be ignored.
     * Provides ABI compatibility with respect to adding new fields.
     */
    uint64_t field_mask;

    /**
     * The field represents the oob resources used by UCG.
     * This field is optional. Corresponding bit is UCG_GROUP_PARAMS_FIELD_OOB_RESOURCE.
     */
    ucg_oob_resource_t oob_resource;
} ucg_global_params_t;

/**
 * @ingroup UCG_GLOBAL
 * @brief Init global resources for UCG.
 *
 * This function must be invoked before any other UCG routines.
 *
 * @param [in]  params  Parameters of ucg global init.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_global_init(const ucg_global_params_t *params);

/**
 * @ingroup UCG_GLOBAL
 * @brief Cleanup global resources.
 *
 *This routine is invoked when the UCG is no longer used.
 */
void ucg_global_cleanup(void);

/**
 * @ingroup UCG_REQUEST
 * @brief UCG request completion callback
 *
 * The callback is invoked whenever the request is completed.
 * Only @ref ucg_request_cleanup is allowed to be called in the callback.
 */
typedef struct ucg_request_complete_cb {
    void (*cb)(void *arg, ucg_status_t status);
    void *arg;
} ucg_request_complete_cb_t;

/**
 * @ingroup UCG_REQUEST
 * @brief Informations for creating collective request.
 */
typedef struct {
    /**
     * Mask of valid fields in this structure, using bits from
     * @ref ucg_request_info_field_t.
     * Fields not specified in this mask will be ignored.
     * Provides ABI compatibility with respect to adding new fields.
     */
    uint64_t field_mask;

    /**
     * Indicates the memory type of buffer used by request. This will determine
     * the plan component used for the request.
     * For barrier request, user must specify an explicit memory type, can not
     * be UCG_MEM_TYPE_UNKNOWN.
     */
    ucg_mem_type_t mem_type;

    /**
     * Specify the request completion callback. For MPI, usually only non-blocking
     * collective operations need to set this field.
     */
    ucg_request_complete_cb_t complete_cb;
} ucg_request_info_t;


/*
 *******************************************************************************
 *                             API declare
 *******************************************************************************
 */
/**
 * @ingroup UCG_BASE
 * @brief Status string.
 *
 * @param status Status code.
 * @return Status string.
 */
const char *ucg_status_string(ucg_status_t status);

/**
 * @ingroup UCG_BASE
 * @brief Query memory attributes
 *
 * @param [in]  ptr     Memory address.
 * @param [out] attr    Attribute.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_mem_query(const void *ptr, ucg_mem_attr_t *attr);

/**
 * @ingroup UCG_DT
 * @brief Create a UCG datatype.
 *
 * @param [in]  params  Parameters of creating datatype.
 * @param [out] dt      Created datatype.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_dt_create(const ucg_dt_params_t *params, ucg_dt_h *dt);

/**
 * @ingroup UCG_DT
 * @brief Destroy a UCG datatype.
 *
 * @param [in] dt       Datatype created by @ref ucg_dt_create
 */
void ucg_dt_destroy(ucg_dt_h dt);

/**
 * @ingroup UCG_DT
 * @brief Create a UCG reduction operation.
 *
 * @param [in]  params  Parameters of creating op.
 * @param [out] op      Created op.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_op_create(const ucg_op_params_t *params, ucg_op_h *op);

/**
 * @ingroup UCG_DT
 * @brief Destroy a UCG reduction operation.
 *
 * @param [in] op       Op created by @ref ucg_op_create
 */
void ucg_op_destroy(ucg_op_h op);

/** @cond TRICK_INTERFACE */
#define UCG_OP_SIZE 32
/**
 * @ingroup UCG_DT
 * @brief Initialize a UCG reduction operation.
 *
 * The main reason why this interface is provided is that the OMPI does not have
 * a proper time to destroy the ucg op created by @ref ucg_op_create.
 *
 * User can allocate the space(stack or heap) of ucg_op_h based on UCG_OP_SIZE
 * and initialize the space by calling this routine. When the initialized op is
 * passed to UCG(e.g. invoking @ref ucg_request_allreduce_init), its value will
 * be saved which means user can release the space of ucg_op_h after the function
 * returned. However, if its type is UCG_OP_TYPE_USER, the params->user_op cannot
 * be released before the request is completed.
 *
 * @note Try not to use this routine.
 *
 * @param [in]  params  Parameters of initializing op.
 * @param [out] op      Initialized op.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_op_init(const ucg_op_params_t *params, ucg_op_h op, uint32_t op_size);
/** @endcond */

/**
 * @ingroup UCG_CONTEXT
 * @brief Read UCG configuration.
 *
 * The routine get the configuration from the runtime environment. The
 * configuration is used to initialize a UCG context.
 * The format of runtime environment is "${USER_ENV_PREFIX}_UCG_${CONFIG_NAME}".
 *
 * @param [in]  env_prefix  User defined prefix of environment. Ignore if NULL.
 * @param [in]  filename    If non-NULL, read configuration from the file.
 * @param [out] config      Configuration descriptor.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_config_read(const char *env_prefix, const char *filename,
                             ucg_config_h *config);

/**
 * @ingroup UCG_CONTEXT
 * @brief Modify the configuration.
 *
 * @param [in] config    Configuration to modify.
 * @param [in] name      Configuration option name.
 * @param [in] value     Value to set.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_config_modify(ucg_config_h config, const char *name,
                               const char *value);

/**
 * @ingroup UCG_CONTEXT
 * @brief Release configuration.
 *
 * @param config Configuration allocated through @ref ucg_config_read
 *               "ucg_config_read()".
 */
void ucg_config_release(ucg_config_h config);

/**
 * @ingroup UCG_CONTEXT
 * @brief Get UCG library version.
 *
 * This routine returns the UCG library version.
 *
 * @param [out] major_version       Filled with library major version.
 * @param [out] minor_version       Filled with library minor version.
 * @param [out] patch_version       Filled with library patch version.
 */
void ucg_get_version(uint32_t *major_version, uint32_t *minor_version,
                     uint32_t *patch_version);

/**
 * @ingroup UCG_CONTEXT
 * @brief Get UCG library version as a string.
 *
 * This routine returns the UCG library version as a string which consists of:
 * "major.minor.patch".
 */
const char *ucg_get_version_string(void);

/** @cond PRIVATE_INTERFACE */
/**
 * @ingroup UCG_CONTEXT
 * @brief Initialize UCG Application Context.
 *
 * This is an internal routine used to check compatibility with a particular
 * API version. @ref ucg_init should be used to create UCG context.
 *
 * @ref ucg_cleanup "ucg_cleanup()" routine.
 *
 * @param [in]  major_version   Library major version.
 * @param [in]  minor_version   Library minor version.
 * @param [in]  params          User defined parameters for the UCG context.
 * @param [in]  config          UCG configuration allocated through
 *                              @ref ucg_config_read "ucg_config_read()" routine.
 * @param [out] context         Initialized UCG context.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_init_version(uint32_t major_version, uint32_t minor_version,
                              const ucg_params_t *params, const ucg_config_h config,
                              ucg_context_h *context);
/** @endcond */

/**
 * @ingroup UCG_CONTEXT
 * @brief Initialize UCG Application Context.
 *
 * UCG Application Context is a concept for managing resources and configurations.
 * Resource isolation is achieved by separating contexts.
 *
 * @ref ucg_cleanup "ucg_cleanup()" routine.
 *
 * @param [in]  params          User defined parameters for the UCG context.
 * @param [in]  config          UCG configuration allocated through
 *                              @ref ucg_config_read "ucg_config_read()" routine.
 * @param [out] context         Initialized UCG context.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
static inline ucg_status_t ucg_init(const ucg_params_t *params,
                                    const ucg_config_h config,
                                    ucg_context_h *context)
{
    return ucg_init_version(UCG_API_MAJOR, UCG_API_MINOR, params, config,
                            context);
}

/**
 * @brief Progress all active request.
 *
 * @param [in] context  UCG context.
 * @return Number of requests completed.
 */
int ucg_progress(ucg_context_h context);

/**
 * @ingroup UCG_CONTEXT
 * @brief Cleanup UCG context.
 *
 * Before invoking this routine, ensure that there are no pending requests and
 * all group are destroyed. Once invoking this routine, the context can not be
 * used anymore.
 *
 * @param context UCG context.
 */
void ucg_cleanup(ucg_context_h context);

/**
 * @ingroup UCG_GROUP
 * @brief Create UCG Communication Group
 *
 * UCG Communication Group consists of a set of processes that perform collective
 * operations. Each process has a UCG Rank(derivative of MPI rank semantics).
 *
 * @ref ucg_group_destroy "ucg_group_destroy" routine.
 *
 * @param [in]  context     UCG context.
 * @param [in]  params      User defined parameters for the UCG group.
 * @param [out] group       Communication group.
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_group_create(ucg_context_h context, const ucg_group_params_t *params,
                              ucg_group_h *group);

/**
 * @ingroup UCG_GROUP
 * @brief Destroy UCG Communication Group
 *
 * @param [in] group    Group created by @ref ucg_group_create.
 */
void ucg_group_destroy(ucg_group_h group);

/**
 * @ingroup UCG_REQUEST
 * @brief Create a persistent broadcast request.
 *
 * Create a request that broadcasts a message from the process with rank root
 * to all processes of the group. It is called by all members of the group using
 * the same arguments for group and root. On return, the content of root's buffer
 * is copied to all other processes.
 *
 * @note The request supports "create once and start many times".
 *
 * @param [inout] buffer    Starting address of buffer
 * @param [in]    count     Number of elements in buffer
 * @param [in]    dt        Data type of elements of buffer
 * @param [in]    root      Rank of broadcast root
 * @param [in]    group     Communication group
 * @param [in]    info      Informations for creating request
 * @param [out]   request   Collective request
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_request_bcast_init(void *buffer, int32_t count, ucg_dt_h dt,
                                    ucg_rank_t root, ucg_group_h group,
                                    const ucg_request_info_t *info,
                                    ucg_request_h *request);

/**
 * @ingroup UCG_REQUEST
 * @brief Create a persistent allreduce request.
 *
 * The request combines the elements provided in the input buffer of each
 * process in the group, using the reduction operation, and returns the combined
 * value in the output buffer of all processes.
 *
 * @note The request supports "create once and start many times".
 *
 * @param [in]  sendbuf     Starting address of send buffer
 * @param [out] recvbuf     Starting address of receive buffer
 * @param [in]  count       Number of elements in send buffer
 * @param [in]  dt          Data type of elements of send buffer
 * @param [in]  op          Operation
 * @param [in]  group       Communication group
 * @param [in]  info        Informations for creating request
 * @param [out] request     Collective request
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_request_allreduce_init(const void *sendbuf, void *recvbuf,
                                        int32_t count, ucg_dt_h dt,
                                        ucg_op_h op, ucg_group_h group,
                                        const ucg_request_info_t *info,
                                        ucg_request_h *request);

/**
 * @ingroup UCG_REQUEST
 * @brief Create a persistent barrier request.
 *
 * The request blocks the caller until all group members have called it. The
 * call returns at any process only after all group members have entered the call.
 *
 * @note The request supports "create once and start many times".
 *
 * @param [in]  group   Communication group
 * @param [in]  info    Informations for creating request
 * @param [out] request Collective request
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_request_barrier_init(ucg_group_h group,
                                      const ucg_request_info_t *info,
                                      ucg_request_h *request);

/**
 * @ingroup UCG_REQUEST
 * @brief Create a persistent alltoallv request.
 *
 * Each process sends distinct data to each of the receivers. The j-th block sent
 * from process i is received by process j and is placed in the i-th block of recvbuf.
 *
 * @note The request supports "create once and start many times".
 *
 * @param [in]  sendbuf         Starting address of send buffer
 * @param [in]  sendcounts      Non-negative integer array (of length group size)
 *                              specifying the number of elements to send to each
 *                              rank
 * @param [in]  sdispls         Integer array (of length group size). Entry i specifies
 *                              the displacement (relative to sendbuf) from which to
 *                              take the outgoing data destined for process i
 * @param [in]  sendtype        Data type of send buffer elements
 * @param [out] recvbuf         Address of receive buffer
 * @param [in]  recvcounts      Non-negative integer array (of length group size)
 *                              specifying the number of elements that can be
 *                              received from each rank
 * @param [in]  rdispls         Integer array (of length group size). Entry i specifies
 *                              the displacement (relative to recvbuf) at which to
 *                              place the incoming data from process i
 * @param [in]  recvtype        Data type of receive buffer elements
 * @param [in]  group           Communication group
 * @param [in]  info            Informations for creating request
 * @param [out] request         Collective request
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_request_alltoallv_init(const void *sendbuf, const int32_t sendcounts[],
                                        const int32_t sdispls[], ucg_dt_h sendtype,
                                        void *recvbuf, const int32_t recvcounts[],
                                        const int32_t rdispls[], ucg_dt_h recvtype,
                                        ucg_group_h group, const ucg_request_info_t *info,
                                        ucg_request_h *request);

/**
 * @ingroup UCG_REQUEST
 * @brief Create a persistent scatterv request.
 *
 * The root process sends a varying count of data to each process. In other words,
 * root sends sendcounts[i] of data to the i-th process.
 *
 * @param [in]  sendbuf         Starting address of send buffer
 * @param [in]  sendcounts      Non-negative integer array (of length group size)
 *                              specifying the number of elements to send to each
 *                              rank
 * @param [in]  displs          Integer array (of length group size). Entry i specifies
 *                              the displacement (relative to sendbuf) from which to
 *                              take the outgoing data destined for process i
 * @param [in]  sendtype        Data type of send buffer elements
 * @param [out] recvbuf         Address of receive buffer
 * @param [in]  recvcount       Number of elements in receive buffer
 * @param [in]  recvtype        Data type of receive buffer elements
 * @param [in]  root            Rank of sending process
 * @param [in]  group           Communication group
 * @param [in]  info            Informations for creating request
 * @param [out] request         Collective request
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_request_scatterv_init(const void *sendbuf, const int32_t *sendcounts,
                                       const int32_t *displs, ucg_dt_h sendtype,
                                       void *recvbuf, int32_t recvcount,
                                       ucg_dt_h recvtype, ucg_rank_t root,
                                       ucg_group_h group, const ucg_request_info_t *info,
                                       ucg_request_h *request);

/**
 * @ingroup UCG_REQUEST
 * @brief Create a persistent gatherv request.
 *
 * Gathers varying amounts of data from all processes to the root process.
 *
 * @param [in]  sendbuf         Starting address of send buffer
 * @param [in]  sendcount       Number of elements in send buffer
 * @param [in]  sendtype        Datatype of send buffer elements
 * @param [out] recvbuf         Address of receive buffer
 * @param [in]  recvcounts      Integer array (of length group size) containing
 *                              the number of elements that are received from each
 *                              process (significant only at root)
 * @param [in]  displs          Integer array (of length group size). Entry i specifies
 *                              the displacement (relative to sendbuf) from which to
 *                              take the outgoing data destined for process i
 * @param [in]  recvtype        Datatype of receive buffer elements
 * @param [in]  root            Rank of receiving process
 * @param [in]  group           Communication group
 * @param [in]  info            Informations for creating request
 * @param [out] request         Collective request
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_request_gatherv_init(const void *sendbuf, const int32_t sendcount,
                                      ucg_dt_h sendtype, void *recvbuf,
                                      const int32_t* recvcounts, const int32_t* displs,
                                      ucg_dt_h recvtype, ucg_rank_t root,
                                      ucg_group_h group, const ucg_request_info_t *info,
                                      ucg_request_h *request);

/**
 * @ingroup UCG_REQUEST
 * @brief Create a persistent allgather request.
 *
 * The block of data sent from the j-th process is received by every process and
 * placed in the j-th block of the recvbuf.
 *
 * @param [in]  sendbuf         Starting address of send buffer
 * @param [in]  sendcount       Number of elements in send buffer(non-negative
 *                              integer)
 * @param [in]  sendtype        Datatype of send buffer elements
 * @param [out] recvbuf         Address of receive buffer
 * @param [in]  recvcounts      Non-negative integer array (of length group size)
 *                              containing the number of elements that are received
 *                              from each process
 * @param [in]  displs          Integer array (of length group size). Entry i specifies
 *                              the displacement (relative to recvbuf) at which to
 *                              place the incoming data from process i
 * @param [in]  recvtype        Datatype of receive buffer elements
 * @param [in]  group           Communication group
 * @param [in]  info            Informations for creating request
 * @param [out] request         Collective request
 * @retval UCG_OK Success.
 * @retval Otherwise Failure.
 */
ucg_status_t ucg_request_allgatherv_init(const void *sendbuf, int sendcount,
                                         ucg_dt_h sendtype, void *recvbuf,
                                         const int32_t *recvcounts, const int32_t *displs,
                                         ucg_dt_h recvtype, ucg_group_h group,
                                         const ucg_request_info_t *info,
                                         ucg_request_h *request);
/**
 * @ingroup UCG_REQUEST
 * @brief Start the request.
 *
 * If start successfully, user need to invoke @ref ucg_request_test to progress
 * the request. If the request is completed, user can start the request again.
 *
 * @note The request may be completed directly in this routine, which means when
 * the request has a complete_cb, it will be invoked here.
 * @note The same collection opmust be performed in the same time sequence in
 * different processes. Otherwise, the collection op requests of different processes
 * cannot be matched.
 *
 * @param [in] request      Collective request
 * @retval UCG_OK Start successfully, next step is to invoke @ref ucg_request_test
 *         to check status and progress the request.
 * @retval UCG_INPROGRESS The request is already started, wrong way to use
 * @retval Otherwise Failed to start the request. The request can only be cleanup
 *         through @ref ucg_request_cleanup
 */
ucg_status_t ucg_request_start(ucg_request_h request);

/**
 * @ingroup UCG_REQUEST
 * @brief Test for the completion of the request.
 *
 * @param [in] request      Collective request
 * @retval UCG_OK The request is completed successfully, can be cleanup or start
 *         again
 * @retval UCG_INPROGRESS The request has not been completed.
 * @retval Otherwise Failed to progress the request. The request can only be cleanup
 *         through @ref ucg_request_cleanup
 */
ucg_status_t ucg_request_test(ucg_request_h request);

/**
 * @ingroup UCG_REQUEST
 * @brief Free the request.
 *
 * This routine is normally used to free inactive persistent requests created
 * with @ref ucg_request_bcast_init and friends. It is also permissible to free
 * an active request (cancel semantics). Once freed, the request can no longer
 * be used.
 *
 * @param [in] request      Collective request
 * @retval UCG_OK Cleanup successfully.
 * @retval UCG_INPROGRESS The request can't be canceled, need to invoke
 *         @ref ucg_request_test to done the request.
 * @retval Otherwise Failed, Usually UCG_ERR_INVALID_PARAM.
 */
ucg_status_t ucg_request_cleanup(ucg_request_h request);

#ifdef __cplusplus
}
#endif

#endif