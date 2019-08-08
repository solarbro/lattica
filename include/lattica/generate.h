#ifndef LATTICA_GENERATE_H
#define LATTICA_GENERATE_H
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    LTC_OK = 0,
    LTC_ERR_INVALIDARGS,
    LTC_ERR_NOSUPPORT,
    LTC_ERR_INTERNAL
} LtcError_t;

typedef enum
{
    LTC_SHAPE_NONE = 0,
    LTC_SHAPE_PLANE,
    LTC_SHAPE_CUBOID,
    LTC_SHAPE_SPHERE,
    LTC_SHAPE_CYLINDER,
    LTC_SHAPE_CONE,
    LTC_SHAPE_PRISM,
    LTC_SHAPE_PYRAMID,
    LTC_SHAPE_TUBE,
    LTC_SHAPE_CAPSULE,
    LTC_SHAPE_TORUS,
    LTC_SHAPE_TORUSKNOT
} LtcShape_t;

typedef enum
{
    LTC_VERTEX_ATTRIB_TYPE_NONE      = 0x0,
    LTC_VERTEX_ATTRIB_TYPE_POSITION  = 0x1,
    LTC_VERTEX_ATTRIB_TYPE_NORMAL    = 0x2,
    LTC_VERTEX_ATTRIB_TYPE_TEXCOORD  = 0x4,
    LTC_VERTEX_ATTRIB_TYPE_TANGENT   = 0x8,
    LTC_VERTEX_ATTRIB_TYPE_BITANGENT = 0x10,
} LtcVertexAttribType_t;

typedef enum
{
    LTC_VERTEX_ATTRIB_SIZE_NONE = 0,
    LTC_VERTEX_ATTRIB_SIZE_FLOAT,
    LTC_VERTEX_ATTRIB_SIZE_FLOAT2,
    LTC_VERTEX_ATTRIB_SIZE_FLOAT3,
    LTC_VERTEX_ATTRIB_SIZE_FLOAT4,
} LtcVertexAttribSize_t;

typedef enum
{
    LTC_INDEX_SIZE_NONE = 0,
    LTC_INDEX_SIZE_8    = 1,
    LTC_INDEX_SIZE_16   = 2,
    LTC_INDEX_SIZE_32   = 4,
} LtcIndexSize_t;

typedef enum 
{
    LTC_UVMAPPING_NONE = 0,
} LtcUvMapping_t;

typedef enum
{
    LTC_WINDING_ORDER_COUNTER_CLOCKWISE = 0,
    LTC_WINDING_ORDER_CLOCKWISE,
} LtcWindingOrder_t;

typedef struct LtcVertexAttribBuffer
{
    void                 *m_buffer;
    uint32_t              m_stride;
    LtcVertexAttribType_t m_attribType;
    LtcVertexAttribSize_t m_attribSize;

    struct LtcVertexAttribBuffer *next;
} LtcVertexAttribBuffer;

typedef struct
{
    void           *m_buffer;
    LtcIndexSize_t  m_indexSize;
} LtcIndexBuffer;

typedef struct
{
    LtcVertexAttribBuffer *m_vertexAttribs;
    uint32_t               m_numVertices;
    LtcIndexBuffer        *m_indices;
    uint32_t               m_numIndices;
} LtcGeometry;

LtcError_t ltcAddVertexAttribBuffer(LtcGeometry *geometry, LtcVertexAttribBuffer *attribBuffer);
LtcError_t ltcRemoveVertexAttribBuffer(LtcVertexAttribType_t attribType);
LtcError_t ltcSetIndexBuffer(LtcGeometry *geometry, LtcIndexBuffer *indexBuffer);

typedef struct
{
    LtcShape_t        m_shape;
    LtcWindingOrder_t m_windingOrder;
    LtcUvMapping_t    m_uvMapping;
} LtcConfig;

void ltcInitDefaultConfig(LtcConfig *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_divX, m_divY;
    float m_sizeX, m_sizeY;
} LtcConfigPlane;

void ltcInitDefaultConfigPlane(LtcConfigPlane *config);

typedef struct 
{
    LtcConfig m_common;
    uint16_t m_divX, m_divY, m_divZ;
    float m_sizeX, m_sizeY, m_sizeZ;
} LtcConfigCuboid;

void ltcInitDefaultConfigCuboid(LtcConfigCuboid *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_divLongitude, m_divLatitude;
    float m_radius;
} LtcConfigSphere;

void ltcInitDefaultConfigSphere(LtcConfigSphere *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_divRadial, m_divAxial, m_divRings;
    float m_length, m_radius;
} LtcConfigCylinder;

void ltcInitDefaultConfigCylinder(LtcConfigCylinder *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_divRadial, m_divAxial, m_divRings;
    float m_radius, m_length;
} LtcConfigCone;

void ltcInitDefaultConfigCone(LtcConfigCone *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_numFacets;
    uint16_t m_divPerFacetRadial, m_divAxial, m_divRings;
    float m_radius, m_length;
} LtcConfigPrism;

void ltcInitDefaultConfigPrism(LtcConfigPrism *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_numFacets;
    uint16_t m_divPerFacetRadial, m_divAxial, m_divRings;
    float m_radius, m_length;
} LtcConfigPyramid;

void ltcInitDefaultConfigPyramid(LtcConfigPyramid *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_divRadial, m_divAxial, m_divRings;
    float m_length, m_outerRadius, m_innerRadius;
} LtcConfigTube;

void ltcInitDefaultConfigTube(LtcConfigTube *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_divRadial, m_divAxial, m_divLatitude;
    float m_radius, m_cylinderLength;
} LtcConfigCapsule;

void ltcInitDefaultConfigCapsule(LtcConfigCapsule *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_divRadialMinor, m_divRadialMajor;
    float m_minorRadius, m_majorRadius;
} LtcConfigTorus;

void ltcInitDefaultConfigTorus(LtcConfigTorus *config);

typedef struct
{
    LtcConfig m_common;
    uint16_t m_divRadial, m_divTubular;
    float m_radius, m_torusRadius, m_tubeRadius;
    int m_p, m_q;
} LtcConfigTorusKnot;

void ltcInitDefaultConfigTorusKnot(LtcConfigTorusKnot *config);

LtcError_t ltcGenerateGeometry(const LtcConfig *config, LtcGeometry *outGeometry);

#ifdef __cplusplus
}
#endif

#endif /* LATTICA_GENERATE_H */