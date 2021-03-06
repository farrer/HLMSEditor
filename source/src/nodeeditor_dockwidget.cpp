/****************************************************************************
**
** Copyright (C) 2016
**
** This file is generated by the Magus toolkit
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

// Include
#include "constants.h"
#include "magus_core.h"
#include "mainwindow.h"
#include "nodeeditor_dockwidget.h"
#include "OgreHlmsPbsDatablock.h"
#include "hlms_pbs_builder.h"
#include "hlms_unlit_builder.h"
#include "hlms_utils_manager.h"

//****************************************************************************/
NodeEditorDockWidget::NodeEditorDockWidget(QString title, MainWindow* parent, Qt::WindowFlags flags) : 
	QDockWidget (title, parent, flags), 
	mParent(parent)
{
    mInnerMain = new QMainWindow();
    mInnerMain->setMinimumSize(100,100);
    setWidget(mInnerMain);

    // Perform standard functions
    createActions();
    createMenus();
    createToolBars();

    // Create the node editor widget.
    mNodeEditor = new Magus::QtNodeEditor(this);
    mNodeEditor->setMenuSelectionToCompoundEnabled(false); // Enabling this makes it a bit more complicated
    mNodeEditor->setMenuExpandCompoundsEnabled(false); // No compounds are used

    connect(mNodeEditor, SIGNAL(nodeToBeRemoved(QtNode*)), this, SLOT(nodeToBeDeleted(QtNode*)));
    connect(mNodeEditor, SIGNAL(nodeSelected(QtNode*)), this, SLOT(nodeSelected(QtNode*)));
    connect(mNodeEditor, SIGNAL(nodeConnected(QtNode*,QtNode*)), this, SLOT(nodeConnected(QtNode*,QtNode*)));
    connect(mNodeEditor, SIGNAL(dropped()), this, SLOT(handleDropped()));
    mInnerMain->setCentralWidget(mNodeEditor);
    mHlmsPbsDatablockNode = 0;
    mHlmsUnlitDatablockNode = 0;
    mCurrentDatablockName = DEFAULT_PBS_DATABLOCK_NAME;
    mHlmsPbsBuilder = new HlmsPbsBuilder(mNodeEditor);
    mHlmsUnlitBuilder = new HlmsUnlitBuilder(mNodeEditor);
    mNodeEditor->setZoom(0.9f);

    // Make some space
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    QRect rec = QApplication::desktop()->screenGeometry();
    setMinimumHeight(0.3 * rec.height());
    //layout()->setSizeConstraint(QLayout::SetMinimumSize);
}

//****************************************************************************/
NodeEditorDockWidget::~NodeEditorDockWidget(void)
{
    delete mHlmsPbsBuilder;
}

//****************************************************************************/
void NodeEditorDockWidget::generateDatablock (void)
{
    doCogHToolbarAction();
}

//****************************************************************************/
void NodeEditorDockWidget::createActions(void)
{
    mNewHlmsPbsDatablockMenuAction = new QAction(QString("New Hlms Pbs"), this);
    connect(mNewHlmsPbsDatablockMenuAction, SIGNAL(triggered()), this, SLOT(doNewHlmsPbsDatablockAction()));
    mNewHlmsUnlitDatablockMenuAction = new QAction(QString("New Hlms Unlit"), this);
    connect(mNewHlmsUnlitDatablockMenuAction, SIGNAL(triggered()), this, SLOT(doNewHlmsUnlitDatablockAction()));
    mNewSamplerblockMenuAction = new QAction(QString("New Texture/Samplerblock"), this);
    connect(mNewSamplerblockMenuAction, SIGNAL(triggered()), this, SLOT(doNewSamplerblockAction()));
    mNewBlendblockMenuAction = new QAction(QString("New Blendblock"), this);
    connect(mNewBlendblockMenuAction, SIGNAL(triggered()), this, SLOT(doNewBlendblockAction()));
    mNewMacroblockMenuAction = new QAction(QString("New Macroblock"), this);
    connect(mNewMacroblockMenuAction, SIGNAL(triggered()), this, SLOT(doNewMacroblockAction()));

    mNewHlmsPbsDatablockHToolbarAction = new QAction(QIcon(ICON_PBS_DATABLOCK), QString("New Hlms Pbs"), this);
    connect(mNewHlmsPbsDatablockHToolbarAction, SIGNAL(triggered()), this, SLOT(doNewHlmsPbsDatablockAction()));
    mNewHlmsUnlitDatablockHToolbarAction = new QAction(QIcon(ICON_UNLIT_DATABLOCK), QString("New Hlms Unlit"), this);
    connect(mNewHlmsUnlitDatablockHToolbarAction, SIGNAL(triggered()), this, SLOT(doNewHlmsUnlitDatablockAction()));
    mNewSamplerblockHToolbarAction = new QAction(QIcon(ICON_SAMPLERBLOCK), QString("New Texture/Samplerblock"), this);
    connect(mNewSamplerblockHToolbarAction, SIGNAL(triggered()), this, SLOT(doNewSamplerblockAction()));
    mNewBlendblockHToolbarAction = new QAction(QIcon(ICON_BLENDBLOCK), QString("New Blendblock"), this);
    connect(mNewBlendblockHToolbarAction, SIGNAL(triggered()), this, SLOT(doNewBlendblockAction()));
    mNewMacroblockHToolbarAction = new QAction(QIcon(ICON_MACROBLOCK), QString("New Macroblock"), this);
    connect(mNewMacroblockHToolbarAction, SIGNAL(triggered()), this, SLOT(doNewMacroblockAction()));
    mCogHToolbarAction = new QAction(QIcon(ICON_GENERATE), QString("Generate Hlms"), this);
    connect(mCogHToolbarAction, SIGNAL(triggered()), this, SLOT(doCogHToolbarAction()));
}

//****************************************************************************/
void NodeEditorDockWidget::createMenus(void)
{
    mDatablockMenu = mInnerMain->menuBar()->addMenu(QString("Hlms"));
    mDatablockMenu->addAction(mNewHlmsPbsDatablockMenuAction);
    mDatablockMenu->addAction(mNewHlmsUnlitDatablockMenuAction);
    mOtherblocksMenu = mInnerMain->menuBar()->addMenu(QString("Other blocks"));
    mOtherblocksMenu->addAction(mNewSamplerblockMenuAction);
    mOtherblocksMenu->addAction(mNewBlendblockMenuAction);
    mOtherblocksMenu->addAction(mNewMacroblockMenuAction);
}

//****************************************************************************/
void NodeEditorDockWidget::createToolBars(void)
{
    mHToolBar = new QToolBar();
    mInnerMain->addToolBar(Qt::TopToolBarArea, mHToolBar);
    mHToolBar->setMinimumHeight(32);
    //mHToolBar->setMinimumWidth(8 * 32);
    mHToolBar->addAction(mNewHlmsPbsDatablockHToolbarAction);
    mHToolBar->addAction(mNewHlmsUnlitDatablockHToolbarAction);
    mHToolBar->addSeparator();
    mHToolBar->addAction(mNewSamplerblockHToolbarAction);
    mHToolBar->addAction(mNewBlendblockHToolbarAction);
    mHToolBar->addAction(mNewMacroblockHToolbarAction);
    mHToolBar->addSeparator();
    mHToolBar->addAction(mCogHToolbarAction);
}

//****************************************************************************/
void NodeEditorDockWidget::newHlmsPbsAndSampler()
{
    // Clear the node editor
    clear();

    // Create a pbs and a sampler for convenience
    doNewHlmsPbsDatablockAction();
    doNewSamplerblockAction();
}

//****************************************************************************/
HlmsNodePbsDatablock* NodeEditorDockWidget::newHlmsPbs(void)
{
    // Clear the node editor
    clear();

    // Create a pbs
    doNewHlmsPbsDatablockAction();
    return mHlmsPbsDatablockNode;
}

//****************************************************************************/
void NodeEditorDockWidget::newHlmsUnlitAndSampler()
{
    // Clear the node editor
    clear();

    // Create an unlit and a sampler for convenience
    doNewHlmsUnlitDatablockAction();
    doNewSamplerblockAction();
}

//****************************************************************************/
HlmsNodeUnlitDatablock* NodeEditorDockWidget::newHlmsUnlit(void)
{
    // Clear the node editor
    clear();

    // Create an unlit
    doNewHlmsUnlitDatablockAction();
    return mHlmsUnlitDatablockNode;
}

//****************************************************************************/
HlmsNodePbsDatablock* NodeEditorDockWidget::createPbsNodeStructure(const HlmsUtilsManager::DatablockStruct& pbsDatablock)
{
    mHlmsPbsDatablockNode = mHlmsPbsBuilder->createPbsNodeStructure(mParent->getOgreManager(), pbsDatablock);
    mHlmsUnlitDatablockNode = 0;
    return mHlmsPbsDatablockNode;
}

//****************************************************************************/
HlmsNodeUnlitDatablock* NodeEditorDockWidget::createUnlitNodeStructure(const HlmsUtilsManager::DatablockStruct& unlitDatablock)
{
    mHlmsUnlitDatablockNode = mHlmsUnlitBuilder->createUnlitNodeStructure(mParent->getOgreManager(), unlitDatablock);
    mHlmsPbsDatablockNode = 0;
    return mHlmsUnlitDatablockNode;
}

//****************************************************************************/
const QString& NodeEditorDockWidget::getCurrentDatablockName(void)
{
    if (mHlmsPbsDatablockNode)
        mCurrentDatablockName = mHlmsPbsDatablockNode->getName();
    else if (mHlmsUnlitDatablockNode)
        mCurrentDatablockName = mHlmsUnlitDatablockNode->getName();

    return mCurrentDatablockName;
}

//****************************************************************************/
HlmsNodePbsDatablock* NodeEditorDockWidget::doNewHlmsPbsDatablockAction(void)
{
    // If mHlmsUnlitDatablockNode, then just return
    if (mHlmsUnlitDatablockNode)
    {
        mParent->mPropertiesDockWidget->setTextureTypePropertyVisible(false);
        mParent->mPropertiesDockWidget->setDetailMapWOSPropertiesVisible(false);
        mParent->mPropertiesDockWidget->setDetailMapAnimationPropertiesVisible(true);
        return mHlmsPbsDatablockNode;
    }

    // Add an mHlmsPbsDatablockNode; only 1 is allowed
    if (!mHlmsPbsDatablockNode)
    {
        mHlmsPbsDatablockNode = mHlmsPbsBuilder->createPbsNode();
        mParent->mPropertiesDockWidget->setTextureTypePropertyVisible(true);
        mParent->mPropertiesDockWidget->setDetailMapWOSPropertiesVisible(true);
        mParent->mPropertiesDockWidget->setDetailMapAnimationPropertiesVisible(false);
        mParent->initCurrentDatablockFileName(); // Don't destroy all other datablocks
        if (mHlmsPbsDatablockNode)
            mHlmsPbsDatablockNode->setSelected(true);
        nodeSelected(mHlmsPbsDatablockNode);
    }

    return mHlmsPbsDatablockNode;
}

//****************************************************************************/
HlmsNodeUnlitDatablock* NodeEditorDockWidget::doNewHlmsUnlitDatablockAction(void)
{
    // If mHlmsPbsDatablockNode, then just return
    if (mHlmsPbsDatablockNode)
    {
        mParent->mPropertiesDockWidget->setTextureTypePropertyVisible(true);
        mParent->mPropertiesDockWidget->setDetailMapWOSPropertiesVisible(true);
        mParent->mPropertiesDockWidget->setDetailMapAnimationPropertiesVisible(false);
        return mHlmsUnlitDatablockNode;
    }

    // Add an mHlmsUnlitDatablockNode; only 1 is allowed
    if (!mHlmsUnlitDatablockNode)
    {
        mHlmsUnlitDatablockNode = mHlmsUnlitBuilder->createUnlitNode();
        mParent->mPropertiesDockWidget->setTextureTypePropertyVisible(false);
        mParent->mPropertiesDockWidget->setDetailMapWOSPropertiesVisible(false);
        mParent->mPropertiesDockWidget->setDetailMapAnimationPropertiesVisible(true);
        mParent->initCurrentDatablockFileName(); // Don't destroy all other datablocks
        if (mHlmsUnlitDatablockNode)
            mHlmsUnlitDatablockNode->setSelected(true);
        nodeSelected(mHlmsUnlitDatablockNode);
    }

    return mHlmsUnlitDatablockNode;
}

//****************************************************************************/
HlmsNodeSamplerblock* NodeEditorDockWidget::doNewSamplerblockAction(void)
{
    HlmsNodeSamplerblock* sampler = mHlmsPbsBuilder->createSamplerNode(mNodeEditor);
    if (!sampler)
        return 0;

    if (mHlmsPbsDatablockNode)
    {
        mParent->mPropertiesDockWidget->setTextureTypePropertyVisible(true);
        mParent->mPropertiesDockWidget->setDetailMapWOSPropertiesVisible(true);
        mParent->mPropertiesDockWidget->setDetailMapAnimationPropertiesVisible(false);
        mHlmsPbsBuilder->connectNodes(mHlmsPbsDatablockNode, sampler);
        nodeConnected(mHlmsPbsDatablockNode, sampler);
    }
    else if (mHlmsUnlitDatablockNode)
    {
        mParent->mPropertiesDockWidget->setTextureTypePropertyVisible(false);
        mParent->mPropertiesDockWidget->setDetailMapWOSPropertiesVisible(false);
        mParent->mPropertiesDockWidget->setDetailMapAnimationPropertiesVisible(true);
        mHlmsUnlitBuilder->connectNodes(mHlmsUnlitDatablockNode, sampler);
        nodeConnected(mHlmsPbsDatablockNode, sampler);
    }

    return sampler;
}

//****************************************************************************/
void NodeEditorDockWidget::doNewBlendblockAction(void)
{
    HlmsNodeBlendblock* blendblock = new HlmsNodeBlendblock(NODE_TITLE_BLENDBLOCK);
    if (!blendblock)
        return;

    blendblock->setType(NODE_TYPE_BLENDBLOCK);
    mNodeEditor->addNode(blendblock);
    if (mHlmsPbsDatablockNode)
        mHlmsPbsBuilder->connectNodes(mHlmsPbsDatablockNode, blendblock);
    else if (mHlmsUnlitDatablockNode)
        mHlmsUnlitBuilder->connectNodes(mHlmsUnlitDatablockNode, blendblock);
}

//****************************************************************************/
void NodeEditorDockWidget::doNewMacroblockAction(void)
{
    HlmsNodeMacroblock* macroblock = new HlmsNodeMacroblock(NODE_TITLE_MACROBLOCK);
    if (!macroblock)
        return;

    macroblock->setType(NODE_TYPE_MACROBLOCK);
    mNodeEditor->addNode(macroblock);
    if (mHlmsPbsDatablockNode)
        mHlmsPbsBuilder->connectNodes(mHlmsPbsDatablockNode, macroblock);
    else if (mHlmsUnlitDatablockNode)
        mHlmsUnlitBuilder->connectNodes(mHlmsUnlitDatablockNode, macroblock);
}

//****************************************************************************/
void NodeEditorDockWidget::doCogHToolbarAction(void)
{
    Ogre::IdString name;
    HlmsUtilsManager* hlmsUtilsManager = mParent->getHlmsUtilsManager();

    // Construct a datablock and set it to the current item in the renderwindow
    if (mHlmsPbsDatablockNode)
    {
        mParent->mPropertiesDockWidget->setTextureTypePropertyVisible(true);
        mParent->mPropertiesDockWidget->setDetailMapWOSPropertiesVisible(true);
        mParent->mPropertiesDockWidget->setDetailMapAnimationPropertiesVisible(false);
        mCurrentDatablockName = mHlmsPbsDatablockNode->getName();
        name = mCurrentDatablockName.toStdString();
        Magus::OgreManager* ogreManager = mParent->getOgreManager();

        QVector<int> indices = mParent->getSubItemIndicesWithDatablockAndReplaceWithDefault(name);
        Ogre::String jsonFileName = hlmsUtilsManager->searchJsonFileName (name); // The datablock may still refer to a loaded material in the material browser
        hlmsUtilsManager->destroyDatablock(name);
        Ogre::HlmsPbsDatablock* datablock = mHlmsPbsBuilder->createPbsDatablock(ogreManager, mHlmsPbsDatablockNode);
        if (datablock)
        {
            mParent->replaceCurrentDatablock(indices, datablock->getName());
            mParent->setCurrentDatablockNames(datablock->getName(), *datablock->getFullName());
            hlmsUtilsManager->addNewDatablockToRegisteredDatablocks(name, jsonFileName);
        }
    }
    else if (mHlmsUnlitDatablockNode)
    {
        mParent->mPropertiesDockWidget->setTextureTypePropertyVisible(false);
        mParent->mPropertiesDockWidget->setDetailMapWOSPropertiesVisible(false);
        mParent->mPropertiesDockWidget->setDetailMapAnimationPropertiesVisible(true);
        mCurrentDatablockName = mHlmsUnlitDatablockNode->getName();
        name = mCurrentDatablockName.toStdString();
        Magus::OgreManager* ogreManager = mParent->getOgreManager();

        QVector<int> indices = mParent->getSubItemIndicesWithDatablockAndReplaceWithDefault(name);
        Ogre::String jsonFileName = hlmsUtilsManager->searchJsonFileName (name); // The datablock may still refer to a loaded material in the material browser
        hlmsUtilsManager->destroyDatablock(name);
        Ogre::HlmsUnlitDatablock* datablock = mHlmsUnlitBuilder->createUnlitDatablock(ogreManager, mHlmsUnlitDatablockNode);
        if (datablock)
        {
            mParent->replaceCurrentDatablock(indices, datablock->getName());
            mParent->setCurrentDatablockNames(datablock->getName(), *datablock->getFullName());
            hlmsUtilsManager->addNewDatablockToRegisteredDatablocks(name, jsonFileName);
        }
    }
}

//****************************************************************************/
void NodeEditorDockWidget::nodeToBeDeleted(QtNode* node)
{
    if (mHlmsPbsDatablockNode == node)
        deleteHlmsPbsDatablockNode();
    else if (mHlmsUnlitDatablockNode == node)
        deleteHlmsUnlitDatablockNode();

    if (node->getType() == NODE_TYPE_SAMPLERBLOCK)
        setBackgroundDiffusePropertyVisibleBasedOnSamplerNodes();

    // Rebuid the datablock again
    generateDatablock();
}

//****************************************************************************/
void NodeEditorDockWidget::deleteHlmsPbsDatablockNode(void)
{
    // When the node is deleted from the node-editor, do not destroy the datablock itself
    // It may be still be applied to one of the items.
    Ogre::IdString name = mHlmsPbsDatablockNode->getName().toStdString();
    mHlmsPbsDatablockNode = 0;
    mParent->initCurrentDatablockFileName();

    generateDatablock();
}

//****************************************************************************/
void NodeEditorDockWidget::deleteHlmsUnlitDatablockNode(void)
{
    // When the node is deleted from the node-editor, do not destroy the datablock itself
    // It may be still be applied to one of the items.
    Ogre::IdString name = mHlmsUnlitDatablockNode->getName().toStdString();
    mHlmsUnlitDatablockNode = 0;
    mParent->initCurrentDatablockFileName();
}

//****************************************************************************/
void NodeEditorDockWidget::nodeSelected(QtNode* node)
{
    if (!node)
    {
        HlmsPropertiesPbsDatablock* hlmsPropertiesPbsDatablock = mParent->mPropertiesDockWidget->mHlmsPropertiesPbsDatablock;
        if (hlmsPropertiesPbsDatablock)
        {
            hlmsPropertiesPbsDatablock->setObject(static_cast<HlmsNodePbsDatablock*>(node));
            mParent->mPropertiesDockWidget->setHlmsPropertiesPbsDatablockVisible(true);
        }
        else
        {
            HlmsPropertiesUnlitDatablock* hlmsPropertiesUnlitDatablock = mParent->mPropertiesDockWidget->mHlmsPropertiesUnlitDatablock;
            if (hlmsPropertiesUnlitDatablock)
            {
                hlmsPropertiesUnlitDatablock->setObject(static_cast<HlmsNodeUnlitDatablock*>(node));
                mParent->mPropertiesDockWidget->setHlmsPropertiesUnlitDatablockVisible(true);
            }
        }
        return;
    }

    if (node->getTitle() == NODE_TITLE_PBS_DATABLOCK)
    {
        HlmsPropertiesPbsDatablock* hlmsPropertiesPbsDatablock = mParent->mPropertiesDockWidget->mHlmsPropertiesPbsDatablock;
        if (hlmsPropertiesPbsDatablock)
        {
            // Determine whether the background colour property is visible
            setBackgroundDiffusePropertyVisibleBasedOnSamplerNodes();

            // Display the pbs properties
            hlmsPropertiesPbsDatablock->setObject(static_cast<HlmsNodePbsDatablock*>(node));
            mParent->mPropertiesDockWidget->setHlmsPropertiesPbsDatablockVisible(true);
        }
    }
    else if (node->getTitle() == NODE_TITLE_UNLIT_DATABLOCK)
    {
        HlmsPropertiesUnlitDatablock* hlmsPropertiesUnlitDatablock = mParent->mPropertiesDockWidget->mHlmsPropertiesUnlitDatablock;
        if (hlmsPropertiesUnlitDatablock)
        {
            hlmsPropertiesUnlitDatablock->setObject(static_cast<HlmsNodeUnlitDatablock*>(node));
            mParent->mPropertiesDockWidget->setHlmsPropertiesUnlitDatablockVisible(true);
        }
    }
    else if (node->getTitle() == NODE_TITLE_SAMPLERBLOCK)
    {
        HlmsPropertiesSamplerblock* hlmsPropertiesSamplerblock = mParent->mPropertiesDockWidget->mHlmsPropertiesSamplerblock;
        if (hlmsPropertiesSamplerblock)
        {
            hlmsPropertiesSamplerblock->setObject(static_cast<HlmsNodeSamplerblock*>(node));
            mParent->mPropertiesDockWidget->setHlmsPropertiesSamplerblockVisible(true);
        }
    }
    else if (node->getTitle() == NODE_TITLE_MACROBLOCK)
    {
        HlmsPropertiesMacroblock* hlmsPropertiesMacroblock = mParent->mPropertiesDockWidget->mHlmsPropertiesMacroblock;
        if (hlmsPropertiesMacroblock)
        {
            hlmsPropertiesMacroblock->setObject(static_cast<HlmsNodeMacroblock*>(node));
            mParent->mPropertiesDockWidget->setHlmsPropertiesMacroblockVisible(true);
        }
    }
    else if (node->getTitle() == NODE_TITLE_BLENDBLOCK)
    {
        HlmsPropertiesBlendblock* hlmsPropertiesBlendblock = mParent->mPropertiesDockWidget->mHlmsPropertiesBlendblock;
        if (hlmsPropertiesBlendblock)
        {
            hlmsPropertiesBlendblock->setObject(static_cast<HlmsNodeBlendblock*>(node));
            mParent->mPropertiesDockWidget->setHlmsPropertiesBlendblockVisible(true);
        }
    }
}

//****************************************************************************/
void NodeEditorDockWidget::nodeConnected(QtNode* baseNode, QtNode* targetNode)
{
    // One of the nodes must be a sampler node; otherwise. don't bother
    HlmsNodeSamplerblock* hlmsNodeSamplerblock = 0;
    if (baseNode  && baseNode->getType() == NODE_TYPE_SAMPLERBLOCK)
        hlmsNodeSamplerblock = static_cast<HlmsNodeSamplerblock*>(baseNode);
    else if (targetNode && targetNode->getType() == NODE_TYPE_SAMPLERBLOCK)
        hlmsNodeSamplerblock = static_cast<HlmsNodeSamplerblock*>(targetNode);
    if (!hlmsNodeSamplerblock)
        return;

    // Ignore the baseNode and targetNode; just check whether the pbs node is available and is attached
    // to a samplernode with a diffuse texture
    setBackgroundDiffusePropertyVisibleBasedOnSamplerNodes();

    // Rebuid the datablock again
    generateDatablock();
}

//****************************************************************************/
EditorHlmsTypes NodeEditorDockWidget::getCurrentDatablockType(void)
{
    if (mHlmsPbsDatablockNode)
        return HLMS_PBS;
    else if (mHlmsUnlitDatablockNode)
        return HLMS_UNLIT;

    return HLMS_NONE;
}

//****************************************************************************/
void NodeEditorDockWidget::handleDropped (void)
{
    // Something was dropped on the node editor. This must be either the texture thumb or and item from the texture tree
    // Use the currently selected item in the tree and create a texture/sampler node
    // nB: When a file from the file explorer is dropped, this function will not take that into account
    // It currently assumes that it came from the texture tree- or thumbs widget
    QString fileName = mParent->mTextureDockWidget->getCurrentFileName();
    if (Magus::fileExist(fileName))
    {
        HlmsNodeSamplerblock* samplerNode = doNewSamplerblockAction();
        if (samplerNode)
        {
            samplerNode->setFileNameTexture(fileName);
            nodeSelected(samplerNode);
        }
    }
    else
    {
        QMessageBox::StandardButton reply = fileDoesNotExistsWarning(fileName);
        if (reply == QMessageBox::Yes)
        {
            mParent->mTextureDockWidget->deleteTexture(fileName);
        }
    }

    // Rebuid the datablock again
    generateDatablock();
}

//****************************************************************************/
void NodeEditorDockWidget::newSamplerblockNode (const QString& fileName)
{
    HlmsNodeSamplerblock* samplerNode = doNewSamplerblockAction();
    if (samplerNode)
    {
        samplerNode->setFileNameTexture(fileName);
        nodeSelected(samplerNode);
    }
}

//****************************************************************************/
QMessageBox::StandardButton NodeEditorDockWidget::fileDoesNotExistsWarning(const QString& fileName)
{
    return QMessageBox::question(0,
                                 "Warning",
                                 fileName + QString(" does not exist. Remove it from the texture browser?"),
                                 QMessageBox::Yes|QMessageBox::No);
}

//****************************************************************************/
void NodeEditorDockWidget::clear (void)
{
    // First delete the current datablock node (because mNodeEditor->clear() doesnt't call nodeToBeDeleted)
    if (mHlmsPbsDatablockNode)
        deleteHlmsPbsDatablockNode();
    else if (mHlmsUnlitDatablockNode)
        deleteHlmsUnlitDatablockNode();

    mNodeEditor->clear();
    mHlmsPbsDatablockNode = 0;
    mHlmsUnlitDatablockNode = 0;
}

//****************************************************************************/
void NodeEditorDockWidget::setBackgroundDiffusePropertyVisibleBasedOnSamplerNodes (void)
{
    if (!mHlmsPbsDatablockNode)
        return;

    // Iterate over all sampler nodes and determine whether one of them applies to a diffuse texture
    QVector<QtNode*> samplerBlockNodes = mHlmsPbsDatablockNode->getNodesConnectedToPorts((PORT_SAMPLERBLOCK));
    HlmsNodeSamplerblock* samplerBlockNode;
    bool diffuseTexture = false;
    foreach(QtNode* node, samplerBlockNodes)
    {
        samplerBlockNode = static_cast<HlmsNodeSamplerblock*>(node);
        if (samplerBlockNode->getTextureType() == 0)
        {
            diffuseTexture = true;
            break;
        }
    }

    HlmsPropertiesPbsDatablock* hlmsPropertiesPbsDatablock = mParent->mPropertiesDockWidget->mHlmsPropertiesPbsDatablock;
    if (!hlmsPropertiesPbsDatablock)
        return;

    if (diffuseTexture)
        hlmsPropertiesPbsDatablock->setBackgroundDiffusePropertyVisible(false);
    else
        hlmsPropertiesPbsDatablock->setBackgroundDiffusePropertyVisible(true);
}

