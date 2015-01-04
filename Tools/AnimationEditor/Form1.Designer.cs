namespace AnimationEditor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.NewAnimMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.LoadImageMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.loadAnimationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAnimationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitEditorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.instructionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.AnimPreviewPanel = new System.Windows.Forms.Panel();
            this.TabControl = new System.Windows.Forms.TabControl();
            this.AnimTab = new System.Windows.Forms.TabPage();
            this.previewAnimButton = new System.Windows.Forms.Button();
            this.delAnimButton = new System.Windows.Forms.Button();
            this.addAnimButton = new System.Windows.Forms.Button();
            this.AnimationsLabel = new System.Windows.Forms.Label();
            this.AnimList = new System.Windows.Forms.ListBox();
            this.AnimInfo = new System.Windows.Forms.TabPage();
            this.FrameNext = new System.Windows.Forms.Button();
            this.FramePrev = new System.Windows.Forms.Button();
            this.AnimStop = new System.Windows.Forms.Button();
            this.AnimPause = new System.Windows.Forms.Button();
            this.AnimPlay = new System.Windows.Forms.Button();
            this.LoopingCheck = new System.Windows.Forms.CheckBox();
            this.AnimInfoFrames = new System.Windows.Forms.Label();
            this.AnimFramesBox = new System.Windows.Forms.TextBox();
            this.AnimInfoImage = new System.Windows.Forms.Label();
            this.AnimImgBox = new System.Windows.Forms.TextBox();
            this.AnimInfoName = new System.Windows.Forms.Label();
            this.AnimInfoNameBox = new System.Windows.Forms.TextBox();
            this.FrameTab = new System.Windows.Forms.TabPage();
            this.deleteFrameButton = new System.Windows.Forms.Button();
            this.createFramebutton = new System.Windows.Forms.Button();
            this.AnimTabFramesLabel = new System.Windows.Forms.Label();
            this.FrameList = new System.Windows.Forms.ListBox();
            this.FrameInfo = new System.Windows.Forms.TabPage();
            this.SaveFrameButton = new System.Windows.Forms.Button();
            this.AddFrameButton = new System.Windows.Forms.Button();
            this.RemoveEvent = new System.Windows.Forms.Button();
            this.AddEvent = new System.Windows.Forms.Button();
            this.EventBox = new System.Windows.Forms.ListBox();
            this.eventLabel = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.FrameNum = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.RenderLeft = new System.Windows.Forms.NumericUpDown();
            this.RenderTop = new System.Windows.Forms.NumericUpDown();
            this.RenderRight = new System.Windows.Forms.NumericUpDown();
            this.RenderBottom = new System.Windows.Forms.NumericUpDown();
            this.AnchorY = new System.Windows.Forms.NumericUpDown();
            this.FrameDur = new System.Windows.Forms.NumericUpDown();
            this.AnchorX = new System.Windows.Forms.NumericUpDown();
            this.AnimImagePanel = new System.Windows.Forms.Panel();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.TabControl.SuspendLayout();
            this.AnimTab.SuspendLayout();
            this.AnimInfo.SuspendLayout();
            this.FrameTab.SuspendLayout();
            this.FrameInfo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FrameNum)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.RenderLeft)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.RenderTop)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.RenderRight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.RenderBottom)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.AnchorY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.FrameDur)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.AnchorX)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1250, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.NewAnimMenu,
            this.LoadImageMenu,
            this.loadAnimationToolStripMenuItem,
            this.saveAnimationToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitEditorToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // NewAnimMenu
            // 
            this.NewAnimMenu.Name = "NewAnimMenu";
            this.NewAnimMenu.Size = new System.Drawing.Size(202, 22);
            this.NewAnimMenu.Text = "&New Animation";
            this.NewAnimMenu.Click += new System.EventHandler(this.toolStripMenuItem2_Click);
            // 
            // LoadImageMenu
            // 
            this.LoadImageMenu.Name = "LoadImageMenu";
            this.LoadImageMenu.Size = new System.Drawing.Size(202, 22);
            this.LoadImageMenu.Text = "&Load Image";
            this.LoadImageMenu.Click += new System.EventHandler(this.loadImageToolStripMenuItem_Click);
            // 
            // loadAnimationToolStripMenuItem
            // 
            this.loadAnimationToolStripMenuItem.Name = "loadAnimationToolStripMenuItem";
            this.loadAnimationToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.loadAnimationToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
            this.loadAnimationToolStripMenuItem.Text = "&Load Animation";
            this.loadAnimationToolStripMenuItem.Click += new System.EventHandler(this.loadAnimationToolStripMenuItem_Click);
            // 
            // saveAnimationToolStripMenuItem
            // 
            this.saveAnimationToolStripMenuItem.Name = "saveAnimationToolStripMenuItem";
            this.saveAnimationToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
            this.saveAnimationToolStripMenuItem.Text = "&Save Animation";
            this.saveAnimationToolStripMenuItem.Click += new System.EventHandler(this.saveAnimationToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(199, 6);
            // 
            // exitEditorToolStripMenuItem
            // 
            this.exitEditorToolStripMenuItem.Name = "exitEditorToolStripMenuItem";
            this.exitEditorToolStripMenuItem.Size = new System.Drawing.Size(202, 22);
            this.exitEditorToolStripMenuItem.Text = "&Exit Editor";
            this.exitEditorToolStripMenuItem.Click += new System.EventHandler(this.exitEditorToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.instructionsToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // instructionsToolStripMenuItem
            // 
            this.instructionsToolStripMenuItem.Name = "instructionsToolStripMenuItem";
            this.instructionsToolStripMenuItem.ShortcutKeys = System.Windows.Forms.Keys.F1;
            this.instructionsToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.instructionsToolStripMenuItem.Text = "&Instructions";
            this.instructionsToolStripMenuItem.Click += new System.EventHandler(this.instructionsToolStripMenuItem_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 24);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.AnimImagePanel);
            this.splitContainer1.Size = new System.Drawing.Size(1250, 675);
            this.splitContainer1.SplitterDistance = 568;
            this.splitContainer1.TabIndex = 1;
            // 
            // splitContainer2
            // 
            this.splitContainer2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.AnimPreviewPanel);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.TabControl);
            this.splitContainer2.Size = new System.Drawing.Size(568, 675);
            this.splitContainer2.SplitterDistance = 197;
            this.splitContainer2.TabIndex = 0;
            // 
            // AnimPreviewPanel
            // 
            this.AnimPreviewPanel.AutoScroll = true;
            this.AnimPreviewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AnimPreviewPanel.Location = new System.Drawing.Point(0, 0);
            this.AnimPreviewPanel.Name = "AnimPreviewPanel";
            this.AnimPreviewPanel.Size = new System.Drawing.Size(566, 195);
            this.AnimPreviewPanel.TabIndex = 0;
            this.AnimPreviewPanel.Scroll += new System.Windows.Forms.ScrollEventHandler(this.AnimPreviewPanel_Scroll);
            this.AnimPreviewPanel.Resize += new System.EventHandler(this.AnimPreviewPanel_Resize);
            // 
            // TabControl
            // 
            this.TabControl.Controls.Add(this.AnimTab);
            this.TabControl.Controls.Add(this.AnimInfo);
            this.TabControl.Controls.Add(this.FrameTab);
            this.TabControl.Controls.Add(this.FrameInfo);
            this.TabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TabControl.Location = new System.Drawing.Point(0, 0);
            this.TabControl.Name = "TabControl";
            this.TabControl.SelectedIndex = 0;
            this.TabControl.Size = new System.Drawing.Size(566, 472);
            this.TabControl.TabIndex = 0;
            // 
            // AnimTab
            // 
            this.AnimTab.Controls.Add(this.previewAnimButton);
            this.AnimTab.Controls.Add(this.delAnimButton);
            this.AnimTab.Controls.Add(this.addAnimButton);
            this.AnimTab.Controls.Add(this.AnimationsLabel);
            this.AnimTab.Controls.Add(this.AnimList);
            this.AnimTab.Location = new System.Drawing.Point(4, 22);
            this.AnimTab.Name = "AnimTab";
            this.AnimTab.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.AnimTab.Size = new System.Drawing.Size(558, 446);
            this.AnimTab.TabIndex = 2;
            this.AnimTab.Text = "Animations";
            this.AnimTab.UseVisualStyleBackColor = true;
            // 
            // previewAnimButton
            // 
            this.previewAnimButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.previewAnimButton.Location = new System.Drawing.Point(328, 334);
            this.previewAnimButton.Name = "previewAnimButton";
            this.previewAnimButton.Size = new System.Drawing.Size(110, 42);
            this.previewAnimButton.TabIndex = 6;
            this.previewAnimButton.Text = "Preview Animation";
            this.previewAnimButton.UseVisualStyleBackColor = true;
            this.previewAnimButton.Click += new System.EventHandler(this.previewAnimButton_Click);
            // 
            // delAnimButton
            // 
            this.delAnimButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.delAnimButton.Location = new System.Drawing.Point(212, 334);
            this.delAnimButton.Name = "delAnimButton";
            this.delAnimButton.Size = new System.Drawing.Size(110, 42);
            this.delAnimButton.TabIndex = 5;
            this.delAnimButton.Text = "Delete Animation";
            this.delAnimButton.UseVisualStyleBackColor = true;
            this.delAnimButton.Click += new System.EventHandler(this.delAnimButton_Click);
            // 
            // addAnimButton
            // 
            this.addAnimButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.addAnimButton.Location = new System.Drawing.Point(96, 334);
            this.addAnimButton.Name = "addAnimButton";
            this.addAnimButton.Size = new System.Drawing.Size(110, 42);
            this.addAnimButton.TabIndex = 4;
            this.addAnimButton.Text = "Add Animation";
            this.addAnimButton.UseVisualStyleBackColor = true;
            this.addAnimButton.Click += new System.EventHandler(this.addAnimButton_Click);
            // 
            // AnimationsLabel
            // 
            this.AnimationsLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnimationsLabel.AutoSize = true;
            this.AnimationsLabel.Location = new System.Drawing.Point(187, 96);
            this.AnimationsLabel.Name = "AnimationsLabel";
            this.AnimationsLabel.Size = new System.Drawing.Size(58, 13);
            this.AnimationsLabel.TabIndex = 1;
            this.AnimationsLabel.Text = "Animations";
            // 
            // AnimList
            // 
            this.AnimList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnimList.FormattingEnabled = true;
            this.AnimList.Location = new System.Drawing.Point(187, 115);
            this.AnimList.Name = "AnimList";
            this.AnimList.Size = new System.Drawing.Size(155, 147);
            this.AnimList.TabIndex = 0;
            this.AnimList.SelectedIndexChanged += new System.EventHandler(this.AnimList_SelectedIndexChanged);
            // 
            // AnimInfo
            // 
            this.AnimInfo.Controls.Add(this.FrameNext);
            this.AnimInfo.Controls.Add(this.FramePrev);
            this.AnimInfo.Controls.Add(this.AnimStop);
            this.AnimInfo.Controls.Add(this.AnimPause);
            this.AnimInfo.Controls.Add(this.AnimPlay);
            this.AnimInfo.Controls.Add(this.LoopingCheck);
            this.AnimInfo.Controls.Add(this.AnimInfoFrames);
            this.AnimInfo.Controls.Add(this.AnimFramesBox);
            this.AnimInfo.Controls.Add(this.AnimInfoImage);
            this.AnimInfo.Controls.Add(this.AnimImgBox);
            this.AnimInfo.Controls.Add(this.AnimInfoName);
            this.AnimInfo.Controls.Add(this.AnimInfoNameBox);
            this.AnimInfo.Location = new System.Drawing.Point(4, 22);
            this.AnimInfo.Name = "AnimInfo";
            this.AnimInfo.Size = new System.Drawing.Size(558, 445);
            this.AnimInfo.TabIndex = 3;
            this.AnimInfo.Text = "Aniamtion Information";
            this.AnimInfo.UseVisualStyleBackColor = true;
            // 
            // FrameNext
            // 
            this.FrameNext.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.FrameNext.Location = new System.Drawing.Point(286, 337);
            this.FrameNext.Name = "FrameNext";
            this.FrameNext.Size = new System.Drawing.Size(110, 42);
            this.FrameNext.TabIndex = 29;
            this.FrameNext.Text = "Next Frame";
            this.FrameNext.UseVisualStyleBackColor = true;
            this.FrameNext.Click += new System.EventHandler(this.FrameNext_Click);
            // 
            // FramePrev
            // 
            this.FramePrev.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.FramePrev.Location = new System.Drawing.Point(170, 337);
            this.FramePrev.Name = "FramePrev";
            this.FramePrev.Size = new System.Drawing.Size(110, 42);
            this.FramePrev.TabIndex = 28;
            this.FramePrev.Text = "Previous Frame";
            this.FramePrev.UseVisualStyleBackColor = true;
            this.FramePrev.Click += new System.EventHandler(this.FramePrev_Click);
            // 
            // AnimStop
            // 
            this.AnimStop.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.AnimStop.Location = new System.Drawing.Point(347, 290);
            this.AnimStop.Name = "AnimStop";
            this.AnimStop.Size = new System.Drawing.Size(122, 41);
            this.AnimStop.TabIndex = 27;
            this.AnimStop.Text = "Stop Animation";
            this.AnimStop.UseVisualStyleBackColor = true;
            this.AnimStop.Click += new System.EventHandler(this.AnimStop_Click);
            // 
            // AnimPause
            // 
            this.AnimPause.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.AnimPause.Location = new System.Drawing.Point(220, 290);
            this.AnimPause.Name = "AnimPause";
            this.AnimPause.Size = new System.Drawing.Size(122, 41);
            this.AnimPause.TabIndex = 26;
            this.AnimPause.Text = "Pause Animation";
            this.AnimPause.UseVisualStyleBackColor = true;
            this.AnimPause.Click += new System.EventHandler(this.AnimPause_Click);
            // 
            // AnimPlay
            // 
            this.AnimPlay.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.AnimPlay.Location = new System.Drawing.Point(92, 290);
            this.AnimPlay.Name = "AnimPlay";
            this.AnimPlay.Size = new System.Drawing.Size(121, 41);
            this.AnimPlay.TabIndex = 25;
            this.AnimPlay.Text = "Play Animation";
            this.AnimPlay.UseVisualStyleBackColor = true;
            this.AnimPlay.Click += new System.EventHandler(this.AnimPlay_Click);
            // 
            // LoopingCheck
            // 
            this.LoopingCheck.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.LoopingCheck.AutoSize = true;
            this.LoopingCheck.Location = new System.Drawing.Point(263, 232);
            this.LoopingCheck.Name = "LoopingCheck";
            this.LoopingCheck.Size = new System.Drawing.Size(99, 17);
            this.LoopingCheck.TabIndex = 24;
            this.LoopingCheck.Text = "Loop Animation";
            this.LoopingCheck.UseVisualStyleBackColor = true;
            this.LoopingCheck.CheckedChanged += new System.EventHandler(this.LoopingCheck_CheckedChanged);
            // 
            // AnimInfoFrames
            // 
            this.AnimInfoFrames.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnimInfoFrames.AutoSize = true;
            this.AnimInfoFrames.Location = new System.Drawing.Point(164, 183);
            this.AnimInfoFrames.Name = "AnimInfoFrames";
            this.AnimInfoFrames.Size = new System.Drawing.Size(93, 13);
            this.AnimInfoFrames.TabIndex = 5;
            this.AnimInfoFrames.Text = "Number of Frames";
            // 
            // AnimFramesBox
            // 
            this.AnimFramesBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnimFramesBox.Location = new System.Drawing.Point(263, 180);
            this.AnimFramesBox.Name = "AnimFramesBox";
            this.AnimFramesBox.Size = new System.Drawing.Size(105, 20);
            this.AnimFramesBox.TabIndex = 4;
            // 
            // AnimInfoImage
            // 
            this.AnimInfoImage.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnimInfoImage.AutoSize = true;
            this.AnimInfoImage.Location = new System.Drawing.Point(164, 209);
            this.AnimInfoImage.Name = "AnimInfoImage";
            this.AnimInfoImage.Size = new System.Drawing.Size(85, 13);
            this.AnimInfoImage.TabIndex = 3;
            this.AnimInfoImage.Text = "Animation Image";
            // 
            // AnimImgBox
            // 
            this.AnimImgBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnimImgBox.Location = new System.Drawing.Point(263, 206);
            this.AnimImgBox.Name = "AnimImgBox";
            this.AnimImgBox.Size = new System.Drawing.Size(105, 20);
            this.AnimImgBox.TabIndex = 2;
            // 
            // AnimInfoName
            // 
            this.AnimInfoName.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnimInfoName.AutoSize = true;
            this.AnimInfoName.Location = new System.Drawing.Point(164, 157);
            this.AnimInfoName.Name = "AnimInfoName";
            this.AnimInfoName.Size = new System.Drawing.Size(84, 13);
            this.AnimInfoName.TabIndex = 1;
            this.AnimInfoName.Text = "Animation Name";
            // 
            // AnimInfoNameBox
            // 
            this.AnimInfoNameBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnimInfoNameBox.Location = new System.Drawing.Point(263, 154);
            this.AnimInfoNameBox.Name = "AnimInfoNameBox";
            this.AnimInfoNameBox.Size = new System.Drawing.Size(105, 20);
            this.AnimInfoNameBox.TabIndex = 0;
            // 
            // FrameTab
            // 
            this.FrameTab.Controls.Add(this.deleteFrameButton);
            this.FrameTab.Controls.Add(this.createFramebutton);
            this.FrameTab.Controls.Add(this.AnimTabFramesLabel);
            this.FrameTab.Controls.Add(this.FrameList);
            this.FrameTab.Location = new System.Drawing.Point(4, 22);
            this.FrameTab.Name = "FrameTab";
            this.FrameTab.Size = new System.Drawing.Size(558, 445);
            this.FrameTab.TabIndex = 4;
            this.FrameTab.Text = "Frames";
            this.FrameTab.UseVisualStyleBackColor = true;
            // 
            // deleteFrameButton
            // 
            this.deleteFrameButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.deleteFrameButton.Location = new System.Drawing.Point(280, 304);
            this.deleteFrameButton.Name = "deleteFrameButton";
            this.deleteFrameButton.Size = new System.Drawing.Size(110, 42);
            this.deleteFrameButton.TabIndex = 10;
            this.deleteFrameButton.Text = "Remove Frame";
            this.deleteFrameButton.UseVisualStyleBackColor = true;
            this.deleteFrameButton.Click += new System.EventHandler(this.deleteFrameButton_Click);
            // 
            // createFramebutton
            // 
            this.createFramebutton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.createFramebutton.Location = new System.Drawing.Point(163, 304);
            this.createFramebutton.Name = "createFramebutton";
            this.createFramebutton.Size = new System.Drawing.Size(110, 42);
            this.createFramebutton.TabIndex = 9;
            this.createFramebutton.Text = "Create Frame";
            this.createFramebutton.UseVisualStyleBackColor = true;
            this.createFramebutton.Click += new System.EventHandler(this.createFramebutton_Click);
            // 
            // AnimTabFramesLabel
            // 
            this.AnimTabFramesLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnimTabFramesLabel.AutoSize = true;
            this.AnimTabFramesLabel.Location = new System.Drawing.Point(197, 62);
            this.AnimTabFramesLabel.Name = "AnimTabFramesLabel";
            this.AnimTabFramesLabel.Size = new System.Drawing.Size(41, 13);
            this.AnimTabFramesLabel.TabIndex = 5;
            this.AnimTabFramesLabel.Text = "Frames";
            // 
            // FrameList
            // 
            this.FrameList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FrameList.FormattingEnabled = true;
            this.FrameList.Location = new System.Drawing.Point(197, 81);
            this.FrameList.Name = "FrameList";
            this.FrameList.Size = new System.Drawing.Size(161, 199);
            this.FrameList.TabIndex = 4;
            this.FrameList.SelectedIndexChanged += new System.EventHandler(this.FrameList_SelectedIndexChanged);
            // 
            // FrameInfo
            // 
            this.FrameInfo.Controls.Add(this.SaveFrameButton);
            this.FrameInfo.Controls.Add(this.AddFrameButton);
            this.FrameInfo.Controls.Add(this.RemoveEvent);
            this.FrameInfo.Controls.Add(this.AddEvent);
            this.FrameInfo.Controls.Add(this.EventBox);
            this.FrameInfo.Controls.Add(this.eventLabel);
            this.FrameInfo.Controls.Add(this.label12);
            this.FrameInfo.Controls.Add(this.FrameNum);
            this.FrameInfo.Controls.Add(this.label7);
            this.FrameInfo.Controls.Add(this.label6);
            this.FrameInfo.Controls.Add(this.label5);
            this.FrameInfo.Controls.Add(this.label4);
            this.FrameInfo.Controls.Add(this.label3);
            this.FrameInfo.Controls.Add(this.label2);
            this.FrameInfo.Controls.Add(this.label1);
            this.FrameInfo.Controls.Add(this.RenderLeft);
            this.FrameInfo.Controls.Add(this.RenderTop);
            this.FrameInfo.Controls.Add(this.RenderRight);
            this.FrameInfo.Controls.Add(this.RenderBottom);
            this.FrameInfo.Controls.Add(this.AnchorY);
            this.FrameInfo.Controls.Add(this.FrameDur);
            this.FrameInfo.Controls.Add(this.AnchorX);
            this.FrameInfo.Location = new System.Drawing.Point(4, 22);
            this.FrameInfo.Name = "FrameInfo";
            this.FrameInfo.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.FrameInfo.Size = new System.Drawing.Size(558, 445);
            this.FrameInfo.TabIndex = 0;
            this.FrameInfo.Text = "Frame Information";
            this.FrameInfo.UseVisualStyleBackColor = true;
            // 
            // SaveFrameButton
            // 
            this.SaveFrameButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.SaveFrameButton.Location = new System.Drawing.Point(268, 383);
            this.SaveFrameButton.Name = "SaveFrameButton";
            this.SaveFrameButton.Size = new System.Drawing.Size(110, 42);
            this.SaveFrameButton.TabIndex = 34;
            this.SaveFrameButton.Text = "Save Frame";
            this.SaveFrameButton.UseVisualStyleBackColor = true;
            this.SaveFrameButton.Click += new System.EventHandler(this.SaveFrameButton_Click);
            // 
            // AddFrameButton
            // 
            this.AddFrameButton.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.AddFrameButton.Location = new System.Drawing.Point(143, 383);
            this.AddFrameButton.Name = "AddFrameButton";
            this.AddFrameButton.Size = new System.Drawing.Size(110, 42);
            this.AddFrameButton.TabIndex = 33;
            this.AddFrameButton.Text = "Add Frame";
            this.AddFrameButton.UseVisualStyleBackColor = true;
            this.AddFrameButton.Click += new System.EventHandler(this.AddFrameButton_Click);
            // 
            // RemoveEvent
            // 
            this.RemoveEvent.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.RemoveEvent.Location = new System.Drawing.Point(152, 316);
            this.RemoveEvent.Name = "RemoveEvent";
            this.RemoveEvent.Size = new System.Drawing.Size(96, 23);
            this.RemoveEvent.TabIndex = 32;
            this.RemoveEvent.Text = "Remove Event";
            this.RemoveEvent.UseVisualStyleBackColor = true;
            this.RemoveEvent.Click += new System.EventHandler(this.RemoveEvent_Click);
            // 
            // AddEvent
            // 
            this.AddEvent.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.AddEvent.Location = new System.Drawing.Point(152, 287);
            this.AddEvent.Name = "AddEvent";
            this.AddEvent.Size = new System.Drawing.Size(96, 23);
            this.AddEvent.TabIndex = 31;
            this.AddEvent.Text = "Add Event";
            this.AddEvent.UseVisualStyleBackColor = true;
            this.AddEvent.Click += new System.EventHandler(this.AddEvent_Click);
            // 
            // EventBox
            // 
            this.EventBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.EventBox.FormattingEnabled = true;
            this.EventBox.Location = new System.Drawing.Point(267, 265);
            this.EventBox.Name = "EventBox";
            this.EventBox.Size = new System.Drawing.Size(125, 43);
            this.EventBox.TabIndex = 30;
            // 
            // eventLabel
            // 
            this.eventLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.eventLabel.AutoSize = true;
            this.eventLabel.Location = new System.Drawing.Point(149, 265);
            this.eventLabel.Name = "eventLabel";
            this.eventLabel.Size = new System.Drawing.Size(92, 13);
            this.eventLabel.TabIndex = 29;
            this.eventLabel.Text = "Attached Event(s)";
            // 
            // label12
            // 
            this.label12.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(147, 38);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(73, 13);
            this.label12.TabIndex = 25;
            this.label12.Text = "Current Frame";
            // 
            // FrameNum
            // 
            this.FrameNum.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FrameNum.Location = new System.Drawing.Point(265, 36);
            this.FrameNum.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.FrameNum.Name = "FrameNum";
            this.FrameNum.Size = new System.Drawing.Size(125, 20);
            this.FrameNum.TabIndex = 24;
            this.FrameNum.ValueChanged += new System.EventHandler(this.FrameNum_ValueChanged);
            // 
            // label7
            // 
            this.label7.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(147, 224);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(104, 13);
            this.label7.TabIndex = 18;
            this.label7.Text = "Render Rect.Bottom";
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(147, 198);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(96, 13);
            this.label6.TabIndex = 17;
            this.label6.Text = "Render Rect.Right";
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(147, 172);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(90, 13);
            this.label5.TabIndex = 16;
            this.label5.Text = "Render Rect.Top";
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(147, 146);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(89, 13);
            this.label4.TabIndex = 15;
            this.label4.Text = "Render Rect.Left";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(147, 113);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(79, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "Frame Duration";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(147, 87);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(78, 13);
            this.label2.TabIndex = 13;
            this.label2.Text = "Anchor Point Y";
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(147, 64);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Anchor Point X";
            // 
            // RenderLeft
            // 
            this.RenderLeft.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RenderLeft.Location = new System.Drawing.Point(265, 144);
            this.RenderLeft.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.RenderLeft.Name = "RenderLeft";
            this.RenderLeft.Size = new System.Drawing.Size(125, 20);
            this.RenderLeft.TabIndex = 6;
            this.RenderLeft.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // RenderTop
            // 
            this.RenderTop.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RenderTop.Location = new System.Drawing.Point(265, 170);
            this.RenderTop.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.RenderTop.Name = "RenderTop";
            this.RenderTop.Size = new System.Drawing.Size(125, 20);
            this.RenderTop.TabIndex = 5;
            this.RenderTop.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // RenderRight
            // 
            this.RenderRight.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RenderRight.Location = new System.Drawing.Point(265, 196);
            this.RenderRight.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.RenderRight.Name = "RenderRight";
            this.RenderRight.Size = new System.Drawing.Size(125, 20);
            this.RenderRight.TabIndex = 4;
            this.RenderRight.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // RenderBottom
            // 
            this.RenderBottom.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RenderBottom.Location = new System.Drawing.Point(265, 222);
            this.RenderBottom.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.RenderBottom.Name = "RenderBottom";
            this.RenderBottom.Size = new System.Drawing.Size(125, 20);
            this.RenderBottom.TabIndex = 3;
            this.RenderBottom.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // AnchorY
            // 
            this.AnchorY.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnchorY.Location = new System.Drawing.Point(265, 88);
            this.AnchorY.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.AnchorY.Name = "AnchorY";
            this.AnchorY.Size = new System.Drawing.Size(125, 20);
            this.AnchorY.TabIndex = 2;
            this.AnchorY.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // FrameDur
            // 
            this.FrameDur.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FrameDur.DecimalPlaces = 2;
            this.FrameDur.Location = new System.Drawing.Point(265, 114);
            this.FrameDur.Name = "FrameDur";
            this.FrameDur.Size = new System.Drawing.Size(125, 20);
            this.FrameDur.TabIndex = 1;
            this.FrameDur.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // AnchorX
            // 
            this.AnchorX.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AnchorX.Location = new System.Drawing.Point(265, 62);
            this.AnchorX.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.AnchorX.Name = "AnchorX";
            this.AnchorX.Size = new System.Drawing.Size(125, 20);
            this.AnchorX.TabIndex = 0;
            this.AnchorX.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // AnimImagePanel
            // 
            this.AnimImagePanel.AutoScroll = true;
            this.AnimImagePanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AnimImagePanel.Location = new System.Drawing.Point(0, 0);
            this.AnimImagePanel.Name = "AnimImagePanel";
            this.AnimImagePanel.Size = new System.Drawing.Size(676, 673);
            this.AnimImagePanel.TabIndex = 0;
            this.AnimImagePanel.Scroll += new System.Windows.Forms.ScrollEventHandler(this.AnimImagePanel_Scroll);
            this.AnimImagePanel.MouseDown += new System.Windows.Forms.MouseEventHandler(this.AnimImagePanel_MouseDown);
            this.AnimImagePanel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.AnimImagePanel_MouseMove);
            this.AnimImagePanel.MouseUp += new System.Windows.Forms.MouseEventHandler(this.AnimImagePanel_MouseUp);
            this.AnimImagePanel.Resize += new System.EventHandler(this.AnimImagePanel_Resize);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1250, 699);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.TabControl.ResumeLayout(false);
            this.AnimTab.ResumeLayout(false);
            this.AnimTab.PerformLayout();
            this.AnimInfo.ResumeLayout(false);
            this.AnimInfo.PerformLayout();
            this.FrameTab.ResumeLayout(false);
            this.FrameTab.PerformLayout();
            this.FrameInfo.ResumeLayout(false);
            this.FrameInfo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.FrameNum)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.RenderLeft)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.RenderTop)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.RenderRight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.RenderBottom)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.AnchorY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.FrameDur)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.AnchorX)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem LoadImageMenu;
        private System.Windows.Forms.ToolStripMenuItem loadAnimationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAnimationToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Panel AnimImagePanel;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.Panel AnimPreviewPanel;
        private System.Windows.Forms.ToolStripMenuItem exitEditorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.TabControl TabControl;
        private System.Windows.Forms.TabPage AnimTab;
        private System.Windows.Forms.Button previewAnimButton;
        private System.Windows.Forms.Button delAnimButton;
        private System.Windows.Forms.Button addAnimButton;
        private System.Windows.Forms.Label AnimationsLabel;
        private System.Windows.Forms.ListBox AnimList;
        private System.Windows.Forms.TabPage FrameInfo;
        private System.Windows.Forms.ListBox EventBox;
        private System.Windows.Forms.Label eventLabel;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.NumericUpDown FrameNum;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown RenderLeft;
        private System.Windows.Forms.NumericUpDown RenderTop;
        private System.Windows.Forms.NumericUpDown RenderRight;
        private System.Windows.Forms.NumericUpDown RenderBottom;
        private System.Windows.Forms.NumericUpDown AnchorY;
        private System.Windows.Forms.NumericUpDown FrameDur;
        private System.Windows.Forms.NumericUpDown AnchorX;
        private System.Windows.Forms.Button RemoveEvent;
        private System.Windows.Forms.Button AddEvent;
        private System.Windows.Forms.TabPage AnimInfo;
        private System.Windows.Forms.TabPage FrameTab;
        private System.Windows.Forms.CheckBox LoopingCheck;
        private System.Windows.Forms.Label AnimInfoFrames;
        private System.Windows.Forms.TextBox AnimFramesBox;
        private System.Windows.Forms.Label AnimInfoImage;
        private System.Windows.Forms.TextBox AnimImgBox;
        private System.Windows.Forms.Label AnimInfoName;
        private System.Windows.Forms.TextBox AnimInfoNameBox;
        private System.Windows.Forms.Button deleteFrameButton;
        private System.Windows.Forms.Button createFramebutton;
        private System.Windows.Forms.Label AnimTabFramesLabel;
        private System.Windows.Forms.ListBox FrameList;
        private System.Windows.Forms.Button AddFrameButton;
        private System.Windows.Forms.Button AnimStop;
        private System.Windows.Forms.Button AnimPause;
        private System.Windows.Forms.Button AnimPlay;
        private System.Windows.Forms.ToolStripMenuItem NewAnimMenu;
        private System.Windows.Forms.Button SaveFrameButton;
        private System.Windows.Forms.Button FrameNext;
        private System.Windows.Forms.Button FramePrev;
        private System.Windows.Forms.ToolStripMenuItem instructionsToolStripMenuItem;
    }
}

