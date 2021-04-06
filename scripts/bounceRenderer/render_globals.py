from maya import cmds, mel


def create_render_globals_node():
    if cmds.objExists("defaultBounceRenderGlobals"):
        return

    cmds.createNode("bounceRenderGlobals",
                    name="defaultBounceRenderGlobals",
                    shared=True,
                    skipSelect=True)


def delete_render_globals_node():
    if cmds.objExists("defaultBounceRenderGlobals"):
        cmds.delete("defaultBounceRenderGlobals")


def create_render_globals_tab():
    print("create_render_globals_tab")

    create_render_globals_node()

    parent_form = cmds.setParent(query=True)
    cmds.setUITemplate("renderGlobalsTemplate", pushTemplate=True)
    cmds.setUITemplate("attributeEditorTemplate", pushTemplate=True)

    cmds.scrollLayout("bounceRendererScrollLayout", horizontalScrollBarThickness=0)
    cmds.columnLayout("bounceRendererColumnLayout", adjustableColumn=True, width=400, rowSpacing=2)

    # ==  Sampling  ==

    cmds.frameLayout("samplesFrameLayout", label="Sampling", collapsable=True, collapse=False)

    cmds.attrFieldSliderGrp("bounceRenderMinSamplesSlider",
                            label="Minimum samples",
                            columnWidth=(3, 160),
                            columnAttach=(1, "left", 4),
                            attribute="defaultBounceRenderGlobals.minSamples")
    cmds.attrFieldSliderGrp("bounceRenderMaxSamplesSlider",
                            label="Maximum samples",
                            columnWidth=(3, 160),
                            columnAttach=(1, "left", 4),
                            attribute="defaultBounceRenderGlobals.maxSamples")
    cmds.attrEnumOptionMenuGrp("bounceRenderSamplerEnum",
                               label="Sampler",
                               columnWidth=(3, 160),
                               columnAttach=(1, "left", 4),
                               attribute="defaultBounceRenderGlobals.sampler")
    cmds.attrFieldSliderGrp("bounceRenderMinAdaptiveThresholdSlider",
                            label="Adaptive threshold",
                            enable=False,
                            columnWidth=(3, 160),
                            columnAttach=(1, "left", 4),
                            attribute="defaultBounceRenderGlobals.adaptiveThreshold")

    cmds.setParent("..")

    # ==  Ray Depth  ==

    cmds.frameLayout("rayDepthFrameLayout", label="Ray Depth", collapsable=True, collapse=False)

    cmds.attrFieldSliderGrp("bounceRenderMaxDepthSlider",
                            label="Max Depth",
                            columnWidth=(3, 160),
                            columnAttach=(1, "left", 4),
                            attribute="defaultBounceRenderGlobals.maxDepth"
                            )
    cmds.separator(height=2)
    cmds.attrFieldSliderGrp("bounceRenderDiffuseDepthSlider",
                            label="Diffuse Depth",
                            columnWidth=(3, 160),
                            columnAttach=(1, "left", 4),
                            attribute="defaultBounceRenderGlobals.diffuseDepth")
    cmds.attrFieldSliderGrp("bounceRenderSpecularDepthSlider",
                            label="Specular Depth",
                            columnWidth=(3, 160),
                            columnAttach=(1, "left", 4),
                            attribute="defaultBounceRenderGlobals.specularDepth")
    cmds.attrFieldSliderGrp("bounceRenderTransmissionDepthSlider",
                            label="Transmission Depth",
                            columnWidth=(3, 160),
                            columnAttach=(1, "left", 4),
                            attribute="defaultBounceRenderGlobals.transmissionDepth")

    cmds.setParent("..")

    # ==  Accelerator  ==

    cmds.frameLayout("acceleratorFrameLayout", label="Accelerator", collapsable=True, collapse=False)

    cmds.attrEnumOptionMenuGrp("bounceRenderAcceleratorEnum",
                               label="Accelerator",
                               columnWidth=(3, 160),
                               columnAttach=(1, "left", 4),
                               attribute="defaultBounceRenderGlobals.accelerator")

    cmds.setParent("..")

    # ==  Threading  ==

    cmds.frameLayout("threadingFrameLayout", label="Threading", collapsable=True, collapse=False)

    cmds.attrFieldSliderGrp("bounceRenderThreadsSlider",
                            label="Threads",
                            columnWidth=(3, 160),
                            columnAttach=(1, "left", 4),
                            attribute="defaultBounceRenderGlobals.threadCount")

    cmds.setUITemplate("renderGlobalsTemplate", popTemplate=True)
    cmds.setUITemplate("attributeEditorTemplate", popTemplate=True)

    cmds.formLayout(parent_form,
                    edit=True,
                    attachForm=[
                        ("bounceRendererScrollLayout", "top", 0),
                        ("bounceRendererScrollLayout", "bottom", 0),
                        ("bounceRendererScrollLayout", "left", 0),
                        ("bounceRendererScrollLayout", "right", 0)])

    create_render_globals_callbacks()


def create_render_globals_callbacks():
    # Disable adaptive threshold,
    cmds.scriptJob(attributeChange=["defaultBounceRenderGlobals.sampler",
                                    "import bounceRender.render_globals; "
                                    "bounceRenderer.render_globals.sampler_changed_callback()"])


def sampler_changed_callback():
    sampler = cmds.getAttr("defaultBounceRenderGlobals.sampler")
    if sampler == 0:
        cmds.attrFieldSliderGrp("bounceRenderMinAdaptiveThresholdSlider",
                                e=True, enable=True)
    else:
        cmds.attrFieldSliderGrp("bounceRenderMinAdaptiveThresholdSlider",
                                e=True, enable=False)


def update_render_globals_tab():
    print("update_render_globals_tab")
    return


# Register create Bounce Renderer Tab
mel.eval("""
global proc createBounceRenderGlobalsTab() {
    python("import bounceRenderer; bounceRenderer.create_render_globals_tab()");
}""")

# Register update Bounce Renderer Tab
mel.eval("""
global proc updateBounceRenderGlobalsTab() {
    python("import bounceRenderer; bounceRenderer.update_render_globals_tab()");
}""")
