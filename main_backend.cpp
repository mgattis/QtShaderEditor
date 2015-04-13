#include <main_backend.h>

QTEST_MAIN( TestBackend )

void TestBackend::projectOpen() {
    bool bResult = project.openProject("C:/Users/Matthew/Documents/QtShaderEditor/QtSEProjects/testProject/testProject.project.json");
    QCOMPARE(bResult, true);
}

void TestBackend::shaderInit() {
    CShader *shader = new CShader("C:/Users/Matthew/Documents/QtShaderEditor/QtSEProjects/testProject/shaders/shader1.shader.json");
    shader->setProjectList(project.getProjectList());

    bool bResult = shader->initialize();

    QCOMPARE(bResult, true);
}

void TestBackend::textureInit() {
    CTexture *texture = new CTexture("C:/Users/Matthew/Documents/QtShaderEditor/QtSEProjects/testProject/textures/texture1.texture.json");
    texture->setProjectList(project.getProjectList());

    bool bResult = texture->initialize();

    QCOMPARE(bResult, true);
}

void TestBackend::modelInit() {
    CModel *model = new CModel("C:/Users/Matthew/Documents/QtShaderEditor/QtSEProjects/testProject/models/model1.model.json");
    model->setProjectList(project.getProjectList());

    bool bResult = model->initialize();

    QCOMPARE(bResult, true);
}

void TestBackend::framebufferInit() {
    CFramebuffer *framebuffer = new CFramebuffer("C:/Users/Matthew/Documents/QtShaderEditor/QtSEProjects/testProject/framebuffers/framebuffer1.framebuffer.json");
    framebuffer->setProjectList(project.getProjectList());

    bool bResult = framebuffer->initialize();

    QCOMPARE(bResult, true);
}

void TestBackend::stageInit() {
    CStage *stage = new CStage("C:/Users/Matthew/Documents/QtShaderEditor/QtSEProjects/testProject/stage1.stage.json");
    stage->setProjectList(project.getProjectList());

    bool bResult = stage->initialize();

    QCOMPARE(bResult, true);
}
