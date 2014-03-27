#include "stdafx.h"
#include "CurveRenderer.h"
#include "ShaderProgram.h"
#include "Resource\ResourceManager.h"
#include "RenderManager.h"
#include "Renderer.h"
#include "Spline\Curve.h"

const CColor CCurveRenderer::GRID_COLOR(0.3f, 0.3f, 0.3f, 0.3f);
const CColor CCurveRenderer::SPLINE_COLOR(1.0f, 0.f, 0.f, 1.f);
const CColor CCurveRenderer::POINT_COLOR(1.0f, 1.0f, 0.f, 1.f);

CCurveRenderer::CCurveRenderer( SharePtr<Spline> spline )
    : m_spline(spline)
    , m_VBOGrid(0)
    , m_VBOSpline(0)
    , m_VBOPoints(0)
{
    CRenderer::GetInstance()->GenBuffers(1, &m_VBOGrid);
    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, m_VBOGrid);
    CRenderer::GetInstance()->BufferData(GL_ARRAY_BUFFER, 
        GRID_VERTEX_NUM*sizeof(CVertexPC), nullptr, GL_DYNAMIC_DRAW);
    CVertexPC *vertices = (CVertexPC *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for(size_t i = 0; i <= GRID_X_MAX/GRID_WIDTH; ++i)
    {
        vertices[i*2].position.x = (float)i*GRID_WIDTH;
        vertices[i*2].position.y = 0.f;
        vertices[i*2].position.z = 0.f;
        vertices[i*2].color = GRID_COLOR;
        vertices[i*2+1].position.x = (float)i*GRID_WIDTH;
        vertices[i*2+1].position.y = (float)GRID_Y_MAX;
        vertices[i*2+1].position.z = 0.f;
        vertices[i*2+1].color = GRID_COLOR;
    }
    for(size_t i = 0; i <= GRID_Y_MAX/GRID_HEIGHT; ++i)
    {
        size_t offset = (GRID_X_MAX/GRID_WIDTH + 1) * 2;
        vertices[offset + i*2].position.x = 0.f;
        vertices[offset + i*2].position.y = (float)i*GRID_HEIGHT;
        vertices[offset + i*2].position.z = 0.f;
        vertices[offset + i*2].color = GRID_COLOR;
        vertices[offset + i*2+1].position.x = (float)GRID_X_MAX;
        vertices[offset + i*2+1].position.y = (float)i*GRID_HEIGHT;
        vertices[offset + i*2+1].position.z = 0.f;
        vertices[offset + i*2+1].color = GRID_COLOR;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, 0);

    CRenderer::GetInstance()->GenBuffers(1, &m_VBOSpline);
    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, m_VBOSpline);
    CRenderer::GetInstance()->BufferData(GL_ARRAY_BUFFER, 
        (SPLINE_SEGMENTS+1)*sizeof(CVertexPC), nullptr, GL_DYNAMIC_DRAW);
    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, 0);

    CRenderer::GetInstance()->GenBuffers(1, &m_VBOPoints);
}

CCurveRenderer::~CCurveRenderer()
{
    CRenderer::GetInstance()->DeleteBuffers(1, &m_VBOPoints);
    CRenderer::GetInstance()->DeleteBuffers(1, &m_VBOSpline);
}

void CCurveRenderer::SetProgram( SharePtr<CShaderProgram> program )
{
    m_program = program;
}

void CCurveRenderer::PreRender()
{
    if(!m_program)
    {
        m_program = CResourceManager::GetInstance()->GetResource<CShaderProgram>(
            _T("..\\SourceCode\\Shader\\vs.txt@..\\SourceCode\\Shader\\fs.txt"), false);
    }
    CRenderer::GetInstance()->UseProgram(m_program->ID());

    CRenderManager::GetInstance()->SetupVPMatrix(true);

    glPointSize(4.0f);
    CRenderer::GetInstance()->DisableGL(GL_DEPTH_TEST);
}

void CCurveRenderer::Render()
{
    renderGrid();
    renderSpline();
    renderPoints();
}

void CCurveRenderer::PostRender()
{
    CRenderer::GetInstance()->EnableGL(GL_DEPTH_TEST);
}

void CCurveRenderer::renderGrid()
{
    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, m_VBOGrid);
    CRenderer::GetInstance()->EnableVertexAttribArray(ATTRIB_INDEX_POSITION);
    CRenderer::GetInstance()->EnableVertexAttribArray(ATTRIB_INDEX_COLOR);
    CRenderer::GetInstance()->VertexAttribPointer(ATTRIB_INDEX_POSITION,
        3, GL_FLOAT, GL_FALSE, sizeof(CVertexPC), (const GLvoid *)offsetof(CVertexPC, position));
    CRenderer::GetInstance()->VertexAttribPointer(ATTRIB_INDEX_COLOR,
        4, GL_FLOAT, GL_FALSE, sizeof(CVertexPC), (const GLvoid *)offsetof(CVertexPC, color));
    CRenderer::GetInstance()->DrawArrays(
        GL_LINES, 0, GRID_VERTEX_NUM);
    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, 0);
}

void CCurveRenderer::renderPoints()
{
    size_t numPoints = m_spline->knots.size();
    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, m_VBOPoints);
    CRenderer::GetInstance()->BufferData( GL_ARRAY_BUFFER, 
        numPoints*sizeof(CVertexPC), nullptr, GL_DYNAMIC_DRAW);
    CVertexPC *vertices = (CVertexPC *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for(size_t i = 0; i < numPoints; ++i)
    {
        vertices[i].position.x = m_spline->knots[i].t;
        vertices[i].position.y = m_spline->knots[i].y;
        vertices[i].position.z = 0.f;
        vertices[i].color = POINT_COLOR;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    CRenderer::GetInstance()->EnableVertexAttribArray(ATTRIB_INDEX_POSITION);
    CRenderer::GetInstance()->EnableVertexAttribArray(ATTRIB_INDEX_COLOR);
    CRenderer::GetInstance()->VertexAttribPointer(ATTRIB_INDEX_POSITION,
        3, GL_FLOAT, GL_FALSE, sizeof(CVertexPC), (const GLvoid *)offsetof(CVertexPC, position));
    CRenderer::GetInstance()->VertexAttribPointer(ATTRIB_INDEX_COLOR,
        4, GL_FLOAT, GL_FALSE, sizeof(CVertexPC), (const GLvoid *)offsetof(CVertexPC, color));
    CRenderer::GetInstance()->DrawArrays(
        GL_POINTS, 0, numPoints);
    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, 0);
}

void CCurveRenderer::renderSpline()
{
    std::unique_ptr<Curve> curve(CurveFactory::createCurve(*m_spline.Get()));

    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, m_VBOSpline);
    CVertexPC *vertices = (CVertexPC *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    float delta = 500.f/SPLINE_SEGMENTS;
    for(int i = 0; i <= SPLINE_SEGMENTS; ++i)
    {
        vertices[i].position.x = delta*i;
        double value;
        curve->getValue(vertices[i].position.x, value);
        vertices[i].position.y = (float)value;
        vertices[i].position.z = 0.f;
        vertices[i].color = SPLINE_COLOR;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    CRenderer::GetInstance()->EnableVertexAttribArray(ATTRIB_INDEX_POSITION);
    CRenderer::GetInstance()->EnableVertexAttribArray(ATTRIB_INDEX_COLOR);
    CRenderer::GetInstance()->VertexAttribPointer(ATTRIB_INDEX_POSITION,
        3, GL_FLOAT, GL_FALSE, sizeof(CVertexPC), (const GLvoid *)offsetof(CVertexPC, position));
    CRenderer::GetInstance()->VertexAttribPointer(ATTRIB_INDEX_COLOR,
        4, GL_FLOAT, GL_FALSE, sizeof(CVertexPC), (const GLvoid *)offsetof(CVertexPC, color));
    CRenderer::GetInstance()->DrawArrays(
        GL_LINE_STRIP, 0, SPLINE_SEGMENTS+1);
    CRenderer::GetInstance()->BindBuffer(GL_ARRAY_BUFFER, 0);
}
