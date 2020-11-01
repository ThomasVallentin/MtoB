from maya import cmds

BOUNCE_RENDERER = "BounceRenderer"
BOUNCE_NAME = "BounceRenderer"


def test():
    print("test")


def register_bounce_renderer():
    cmds.renderer(BOUNCE_RENDERER,
                  rendererUIName=BOUNCE_NAME,
                  # iprRenderProcedure='mayaSoftwareIprRender',
                  # iprOptionsProcedure='',
                  # isRunningIprProcedure='mayaSoftwareIsRunningIpr',
                  # startIprRenderProcedure='mayaSoftwareStartIprRender',
                  # stopIprRenderProcedure='mayaSoftwareStopIprRender',
                  # refreshIprRenderProcedure='mayaSoftwareRefreshIprImage',
                  # pauseIprRenderProcedure='mayaSoftwarePauseIprRender',
                  # changeIprRegionProcedure='mayaSoftwareChangeIprRegion',
                  # iprOptionsMenuLabel='IPR Tuning Options',
                  # iprOptionsSubMenuProcedure='mayaSoftwareIprUpdateOptionsSubMenu',
                  # iprRenderSubMenuProcedure='mayaSoftwareIprUpdateRenderSubMenu',
                  renderProcedure='bounceRender',
                  # renderOptionsProcedure='RenderOptions',
                  # renderDiagnosticsProcedure='RenderDiagnostics',
                  # commandRenderProcedure='render -batch',
                  # batchRenderProcedure='BatchRender',
                  # batchRenderOptionsProcedure='BatchRenderOptions',
                  # batchRenderOptionsStringProcedure="mayaSoftwareBatchRenderOptionsString",
                  # cancelBatchRenderProcedure='CancelBatchRender',
                  # showBatchRenderProcedure='ShowBatchRender',
                  # showRenderLogProcedure='',
                  # showBatchRenderLogProcedure='',
                  # renderRegionProcedure='mayaRenderRegion',
                  # textureBakingProcedure='performConvertSolidTx',
                  # polyPrelightProcedure='performPrelight',
                  # renderingEditorsSubMenuProcedure='',
                  # logoImageName='',
                  # logoCallbackProcedure=''
                  )

    # Add common render globals node & tab
    cmds.renderer(BOUNCE_RENDERER, e=True, addGlobalsNode="defaultRenderGlobals")
    cmds.renderer(BOUNCE_RENDERER, e=True, addGlobalsTab=("Common",
                                                          "createMayaSoftwareCommonGlobalsTab",
                                                          "updateMayaSoftwareGlobalsTab"))

    # Add bounce specific render globals node & tab
    cmds.renderer(BOUNCE_RENDERER, e=True, addGlobalsNode=("python(\\\"import bounceRenderer; "
                                                           "bounceRenderer.create_render_globals_node()\\\")"))
    cmds.renderer(BOUNCE_RENDERER, e=True, addGlobalsTab=(BOUNCE_NAME,
                                                          ("python(\\\"import bounceRenderer; "
                                                           "bounceRenderer.create_render_globals_tab()\\\")"),
                                                          ("python(\\\"import bounceRenderer; "
                                                           "bounceRenderer.update_render_globals_tab()\\\")")))


def deregister_bounce_renderer():
    cmds.renderer(BOUNCE_RENDERER, unregisterRenderer=True)
