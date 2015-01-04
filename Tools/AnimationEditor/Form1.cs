using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using System.IO;

namespace AnimationEditor
{
    public partial class Form1 : Form
    {
        SGP.CSGP_Direct3D p_D3D = SGP.CSGP_Direct3D.GetInstance();
        SGP.CSGP_TextureManager p_TM = SGP.CSGP_TextureManager.GetInstance();
        string animImgPath = null;
        int AnchorPtX, AnchorPtY;
        int currentAnimation, currentFrame;
        int renderLeft, renderTop, renderRight, renderBottom;
        int rectLeft, rectTop, rectRight, rectBottom;
        float frameElapsedTime;
        float frameDuration;
        bool animLooping, previewAnimation, playAnimation, animationPlayed, drawRenderRect, drawAnchorPt, renderRightSet, renderBottomSet, renderLeftSet, renderTopSet;
        bool InitializedComponents = false;
        Point animImgOffset, animPrevOffset;
        List<Animations> editAnimations;

        int animImageID = -1;

        public class AnimFrames
        {
            int anchorPointX, anchorPointY;
            float currFrameDuration;
            int renderLeft, renderTop, renderRight, renderBottom;
            string frameEvent;

            public string FrameEvent
            {
                get { return frameEvent; }
                set { frameEvent = value; }
            }

            public int RenderLeft
            {
                get { return renderLeft; }
                set { renderLeft = value; }
            }

            public int RenderTop
            {
                get { return renderTop; }
                set { renderTop = value; }
            }

            public int RenderRight
            {
                get { return renderRight; }
                set { renderRight = value; }
            }

            public int RenderBottom
            {
                get { return renderBottom; }
                set { renderBottom = value; }
            }

            public int AnchorPointX
            {
                get { return anchorPointX; }
                set { anchorPointX = value; }
            }

            public int AnchorPointY
            {
                get { return anchorPointY; }
                set { anchorPointY = value; }
            }

            public float CurrFrameDuration
            {
                get { return currFrameDuration; }
                set { currFrameDuration = value; }
            }

            Rectangle renderRect;

            public Rectangle RenderRect
            {
                get { return renderRect; }
                set { renderRect = value; }
            }
        }

        public class Animations
        {
            List<AnimFrames> animationFrames;

            public List<AnimFrames> AnimationFrames
            {
                get { return animationFrames; }
                set { animationFrames = value; }
            }

            bool isLooping;
            string animationName;
            int animImageID;

            public int AnimImageID
            {
                get { return animImageID; }
                set { animImageID = value; }
            }

            public bool IsLooping
            {
                get { return isLooping; }
                set { isLooping = value; }
            }

            public string AnimationName
            {
                get { return animationName; }
                set { animationName = value; }
            }
        }

        public Form1()
        {
            InitializeComponent();
            currentAnimation = 0;
            currentFrame = 0;
            frameElapsedTime = 0.0f;
            previewAnimation = false;
            renderLeftSet = false;
            renderTopSet = false;
            renderRightSet = false;
            renderBottomSet = false;
            editAnimations = new List<Animations>();

            p_D3D.Initialize(AnimImagePanel, false);
            p_D3D.AddRenderTarget(AnimPreviewPanel);
            p_TM.Initialize(p_D3D.Device, p_D3D.Sprite);
            InitializedComponents = true;
        }

        private void loadImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // open a dialog box
            OpenFileDialog odialog = new OpenFileDialog();
            odialog.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\Game\\Resources\\Images");
            // catch the dialog box result
            if (odialog.ShowDialog() == DialogResult.OK)
            {
                if (animImageID != -1)
                    p_TM.UnloadTexture(animImageID);

                animImgPath = odialog.FileName;
                animImageID = p_TM.LoadTexture(animImgPath);
                if (editAnimations.Count != 0)
                    editAnimations[currentAnimation].AnimImageID = animImageID;
                AnimImgBox.Text = Path.GetFileName(animImgPath);
                animImgPath = Path.GetFileName(animImgPath);
            }

            if (animImageID != -1)
                AnimImagePanel.AutoScrollMinSize = new Size(p_TM.GetTextureHeight(animImageID), p_TM.GetTextureWidth(animImageID));

        }

        private bool looping = true;

        public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        }

        public void Update(float fElapsedTime)
        {
            frameElapsedTime += fElapsedTime;

            if (editAnimations.Count != 0)
            {
                if (previewAnimation == true || playAnimation == true)
                {
                    if (editAnimations[currentAnimation].AnimImageID != -1)
                        this.animImageID = editAnimations[currentAnimation].AnimImageID;

                    if (frameElapsedTime > editAnimations[currentAnimation].AnimationFrames[currentFrame].CurrFrameDuration)
                    {
                        frameElapsedTime = 0.0f;
                        if (previewAnimation == true && animationPlayed == false
                            || playAnimation == true && editAnimations[currentAnimation].IsLooping == true
                            || playAnimation == true && animationPlayed == false)
                            ++currentFrame;
                    }

                    if (currentFrame == editAnimations[currentAnimation].AnimationFrames.Count)
                    {
                        currentFrame = 0;
                        animationPlayed = true;
                        if (previewAnimation == true)
                            animationPlayed = false;
                        previewAnimation = false;
                    }

                    UpdateAnimInfo();
                    UpdateAnimFramesInfo();
                    UpdateInformation();
                }
            }
        }

        public void UpdateInformation()
        {
            if (editAnimations != null && currentAnimation != 0)
            {
                if (editAnimations[currentAnimation].AnimationFrames.Count != 0)
                {
                    this.AnchorPtX = (int)(AnchorX.Value) + editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft + this.animImgOffset.X;
                    this.AnchorPtY = (int)(AnchorY.Value) + editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop + this.animImgOffset.Y;


                    this.frameDuration = (float)(FrameDur.Value);

                    this.renderLeft = (int)(RenderLeft.Value);
                    this.renderTop = (int)(RenderTop.Value);
                    this.renderRight = (int)(RenderRight.Value);
                    this.renderBottom = (int)(RenderBottom.Value);

                    if (LoopingCheck.Checked == true)
                        animLooping = true;
                    else if (LoopingCheck.Checked == false)
                        animLooping = false;
                }
            }
        }

        public void UpdateAnimInfo()
        {
            if (editAnimations.Count == 0)
            {
                AnimList.Items.Clear();
                AnimInfoNameBox.Text = null;
                AnimFramesBox.Text = null;
                AnimImgBox.Text = null;

                LoopingCheck.Checked = false;
            }
            else if (editAnimations.Count > 0)
            {
                editAnimations[currentAnimation].AnimImageID = animImageID;

                AnimInfoNameBox.Text = editAnimations[currentAnimation].AnimationName;
                if (editAnimations[currentAnimation].AnimationFrames != null)
                {
                    AnimFramesBox.Text = Convert.ToString(editAnimations[currentAnimation].AnimationFrames.Count);
                }
                AnimImgBox.Text = Path.GetFileName(animImgPath);

                if (editAnimations[currentAnimation].IsLooping == true)
                {
                    LoopingCheck.Checked = true;
                    animLooping = true;
                }
                else if (editAnimations[currentAnimation].IsLooping == false)
                {
                    LoopingCheck.Checked = false;
                    animLooping = false;
                }
            }
        }

        public void UpdateAnimFramesInfo()
        {
            if (editAnimations.Count == 0)
            {
                FrameList.Items.Clear();

                AnchorX.Value = 0;
                AnchorY.Value = 0;
                FrameDur.Value = 0;

                RenderLeft.Value = 0;
                RenderTop.Value = 0;
                RenderRight.Value = 0;
                RenderBottom.Value = 0;

                EventBox.Items.Clear();
            }
            else if (editAnimations.Count > 0)
            {
                if (this.currentFrame != -1 && editAnimations[currentAnimation].AnimationFrames != null)
                {
                    FrameNum.Value = this.currentFrame;
                    FrameList.SelectedIndex = this.currentFrame;
                }

                if (editAnimations[currentAnimation].AnimationFrames != null)
                {
                    AnchorX.Value = editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointX;
                    AnchorY.Value = editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointY;
                    FrameDur.Value = Convert.ToDecimal(editAnimations[currentAnimation].AnimationFrames[currentFrame].CurrFrameDuration);

                    RenderLeft.Value = editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft;
                    RenderTop.Value = editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop;
                    RenderRight.Value = editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRight;
                    RenderBottom.Value = editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderBottom;

                    EventBox.Items.Clear();
                    EventBox.Items.Add(editAnimations[currentAnimation].AnimationFrames[currentFrame].FrameEvent);
                }
            }
        }

        public void UpdateEditAnimation()
        {
            editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointX = (int)AnchorX.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointY = (int)AnchorY.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].CurrFrameDuration = (float)FrameDur.Value;

            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft = (int)RenderLeft.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop = (int)RenderTop.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRight = (int)RenderRight.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderBottom = (int)RenderBottom.Value;

            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRect = new Rectangle(
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft,
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop,
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRight - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft,
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderBottom - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop);

            //editAnimations[currentAnimation].AnimationFrames[currentFrame].FrameEvent = EventBox.Items[0].ToString();
        }

        public void Render()
        {
            p_D3D.Clear(AnimImagePanel, Color.Gray);

            p_D3D.DeviceBegin();
            p_D3D.SpriteBegin();

            if (animImageID != -1)
                p_TM.Draw(animImageID, animImgOffset.X, animImgOffset.Y);

            if (this.drawRenderRect == true)
            {
                p_D3D.DrawHollowRect(new Rectangle(
                    this.renderLeft + animImgOffset.X,
                    this.renderTop + animImgOffset.Y,
                    this.renderRight - this.renderLeft,
                    this.renderBottom - this.renderTop), Color.Cyan, 1);
            }

            if (this.drawAnchorPt == true)
            {
                Rectangle render = new Rectangle(
                    this.AnchorPtX - 2 + animImgOffset.X,
                    this.AnchorPtY - 2 + animImgOffset.Y,
                    4,
                    4);
                p_D3D.DrawRect(render, Color.Yellow);
            }

            if (editAnimations.Count != 0)
            {
                if (this.drawRenderRect == false)
                {
                    Rectangle renderRect = new Rectangle(
                    this.renderLeft + animImgOffset.X,
                    this.renderTop + animImgOffset.Y,
                    this.renderRight - this.renderLeft,
                    this.renderBottom - this.renderTop);
                    p_D3D.DrawHollowRect(renderRect, Color.Cyan, 1);
                }

                if (this.drawAnchorPt == false)
                {
                    Rectangle anchorpoint = new Rectangle(
                        this.AnchorPtX - 2 + animImgOffset.X,
                        this.AnchorPtY - 2 + animImgOffset.Y,
                        4,
                        4);
                    p_D3D.DrawRect(anchorpoint, Color.Yellow);
                }
            }

            p_D3D.SpriteEnd();
            p_D3D.DeviceEnd();
            p_D3D.Present();

            p_D3D.Clear(AnimPreviewPanel, Color.Gray);

            p_D3D.DeviceBegin();
            p_D3D.SpriteBegin();

            // DRAW STUFF if there is an animation to preview
            if (editAnimations.Count != 0
                && editAnimations[currentAnimation].AnimImageID != -1)
            {
                if (editAnimations[currentAnimation].AnimationFrames != null)
                {
                    // draw center of preview with offset based on anchor pt.
                    if (editAnimations[currentAnimation].AnimationFrames.Count > 0)
                        p_TM.Draw(editAnimations[currentAnimation].AnimImageID,
                           (AnimPreviewPanel.Width / 2) - editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointX,
                           (3 * AnimPreviewPanel.Height / 4) - editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointY,
                           1.0f, 1.0f, editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRect);

                    // draw the render rect around the preview
                    if (editAnimations[currentAnimation].AnimationFrames.Count > 0)
                        p_D3D.DrawHollowRect(new Rectangle(
                                                       (AnimPreviewPanel.Width / 2) - editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointX,
                           (3 * AnimPreviewPanel.Height / 4) - editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointY,
                           editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRight - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft,
                           editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderBottom - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop
                            ), Color.Cyan, 1);
                    // draw a rect at the anchor pt.
                    p_D3D.DrawRect(new Rectangle((AnimPreviewPanel.Width / 2) - 2, (3 * AnimPreviewPanel.Height / 4) - 2, 4, 4), Color.Yellow);

                    // information strings
                    string animRectLeft = null, animRectRight = null, animRectTop = null, animRectBottom = null, animAnchorPt = null, animEvent = null, animDuration = null;
                    int anchorOffsetX, anchorOffsetY;

                    anchorOffsetX = this.AnchorPtX - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft;
                    anchorOffsetY = this.AnchorPtY - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop;

                    // show stats in the preview window about the previewd animation
                    //animAnchorPt = "Anchor Point: " + anchorOffsetX.ToString() + ", " + anchorOffsetY.ToString();
                    //animRectLeft = "Render Rect. Left: " + editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft.ToString();
                    //animRectRight = "Render Rect. Right: " + editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRight.ToString();
                    //animRectTop = "Render Rect. Top: " + editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop.ToString();
                    //animRectBottom = "Render Rect. Bottom: " + editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderBottom.ToString();
                    //animEvent = "Event: " + editAnimations[currentAnimation].AnimationFrames[currentFrame].FrameEvent.ToString();
                    //animDuration = "Duration: " + editAnimations[currentAnimation].AnimationFrames[currentFrame].CurrFrameDuration.ToString() + "s";

                    //Font m_Font = new Font("Arial", 11);
                    //p_D3D.DrawText(animAnchorPt, 0, 0, Color.Pink);
                    //p_D3D.DrawText(animRectLeft, 0, 20, Color.Pink);
                    //p_D3D.DrawText(animRectTop, 0, 40, Color.Pink);
                    //p_D3D.DrawText(animRectRight, 0, 60, Color.Pink);
                    //p_D3D.DrawText(animRectBottom, 0, 80, Color.Pink);
                    //p_D3D.DrawText(animDuration, 0, 100, Color.Pink);
                    //p_D3D.DrawText(animEvent, 0, 120, Color.Pink);
                }
            }

            p_D3D.SpriteEnd();
            p_D3D.DeviceEnd();
            p_D3D.Present();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            looping = false;
        }

        private void exitEditorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void AnimImagePanel_Scroll(object sender, ScrollEventArgs e)
        {
            animImgOffset = new Point(0, 0);
            animImgOffset.X += AnimImagePanel.AutoScrollPosition.X;
            animImgOffset.Y += AnimImagePanel.AutoScrollPosition.Y;

            Render();
        }

        private void loadAnimationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // open a dialog box
            OpenFileDialog odialog = new OpenFileDialog();
            odialog.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\Game\\Animations");

            // catch the dialog box result
            if (odialog.ShowDialog() == DialogResult.OK)
            {
                frameElapsedTime = 0.0f;

                string animFileName = null;
                if (editAnimations.Count != 0)
                    editAnimations.Clear();

                animFileName = odialog.FileName;

                XmlReader input = XmlReader.Create(animFileName);

                // Setting the Animation Name
                input.ReadToFollowing("Animation");
                while (input.Name.Equals("Animation") && input.NodeType == XmlNodeType.Element)
                {
                    Animations currAnimation = new Animations();
                    input.MoveToFirstAttribute();
                    currAnimation.AnimationName = input.Value;

                    // Setting the Animation Looping
                    input.MoveToNextAttribute();
                    int looping = Convert.ToInt32(input.Value);
                    if (looping == 0)
                        currAnimation.IsLooping = false;
                    else if (looping == 1)
                        currAnimation.IsLooping = true;

                    // Set the image ID
                    input.MoveToNextAttribute();
                    animImgPath = input.Value;

                    string imageID = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory()) + "\\..\\..\\Game\\Resources\\Images\\" + animImgPath;

                    currAnimation.AnimImageID =
                        p_TM.LoadTexture(imageID);
                    animImageID = currAnimation.AnimImageID;

                    if (animImageID != -1)
                        AnimImagePanel.AutoScrollMinSize = new Size(p_TM.GetTextureHeight(animImageID), p_TM.GetTextureWidth(animImageID));

                    currAnimation.AnimationFrames = new List<AnimFrames>();

                    // Add Frames
                    input.ReadToFollowing("AnimFrames");
                    while (input.Name.Equals("AnimFrames") && input.NodeType == XmlNodeType.Element
                        && input != null)
                    {
                        // Create a frame
                        AnimFrames currentFrame = new AnimFrames();

                        // Add Anchor Points
                        input.MoveToFirstAttribute();
                        currentFrame.AnchorPointX = Convert.ToInt32(input.Value);
                        input.MoveToNextAttribute();
                        currentFrame.AnchorPointY = Convert.ToInt32(input.Value);

                        // Add Frame Duration
                        input.MoveToNextAttribute();
                        currentFrame.CurrFrameDuration = (float)Convert.ToDouble(input.Value);

                        // Rectangle points
                        input.MoveToNextAttribute();
                        currentFrame.RenderLeft = (int)Convert.ToDouble(input.Value);
                        input.MoveToNextAttribute();
                        currentFrame.RenderTop = (int)Convert.ToDouble(input.Value);
                        input.MoveToNextAttribute();
                        currentFrame.RenderRight = (int)Convert.ToDouble(input.Value);
                        input.MoveToNextAttribute();
                        currentFrame.RenderBottom = (int)Convert.ToDouble(input.Value);

                        input.MoveToNextAttribute();
                        currentFrame.FrameEvent = input.Value;

                        // Create Rectangles
                        currentFrame.RenderRect = new Rectangle(currentFrame.RenderLeft, currentFrame.RenderTop,
                            currentFrame.RenderRight - currentFrame.RenderLeft, currentFrame.RenderBottom - currentFrame.RenderTop);

                        currAnimation.AnimationFrames.Add(currentFrame);
                        input.ReadToNextSibling("AnimFrames");
                    }

                    editAnimations.Add(currAnimation);
                    input.ReadToNextSibling("Animation");
                }
            }

            if (editAnimations.Count > 0)
            {
                AnimList.Items.Clear();
                for (int i = 0; i < editAnimations.Count; ++i)
                    AnimList.Items.Add(editAnimations[i].AnimationName);

                FrameList.Items.Clear();
                if (editAnimations[currentAnimation].AnimationFrames != null)
                {
                    if (editAnimations[currentAnimation].AnimationFrames.Count > 0)
                    {
                        for (int i = 0; i < editAnimations[currentAnimation].AnimationFrames.Count; ++i)
                            FrameList.Items.Add(i + 1);

                        FrameList.SelectedIndex = this.currentFrame;
                    }
                }

                AnimList.SelectedIndex = this.currentAnimation;

                UpdateAnimInfo();
                UpdateAnimFramesInfo();
                UpdateInformation();
            }
        }

        private void AnimList_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.currentAnimation = AnimList.SelectedIndex;

            FrameList.Items.Clear();
            if (editAnimations[currentAnimation].AnimationFrames != null
                && editAnimations[currentAnimation].AnimationFrames.Count > 0)
            {
                for (int i = 0; i < editAnimations[currentAnimation].AnimationFrames.Count; ++i)
                    FrameList.Items.Add(i + 1);
            }

            UpdateAnimInfo();
            UpdateAnimFramesInfo();
            UpdateInformation();
        }

        private void FrameList_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.currentFrame = FrameList.SelectedIndex;
            UpdateAnimFramesInfo();
            UpdateInformation();
        }

        private void FramePrev_Click(object sender, EventArgs e)
        {
            --this.currentFrame;
            if (this.currentFrame < 0)
                this.currentFrame = editAnimations[currentAnimation].AnimationFrames.Count - 1;

            UpdateAnimFramesInfo();
            UpdateInformation();
        }

        private void FrameNext_Click(object sender, EventArgs e)
        {
            ++this.currentFrame;
            if (this.currentFrame == editAnimations[currentAnimation].AnimationFrames.Count)
                this.currentFrame = 0;

            UpdateAnimFramesInfo();
            UpdateInformation();
        }

        private void saveAnimationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // check to make sure Animations are present
            if (editAnimations == null || editAnimations.Count == 0)
                throw new System.ArgumentException("There are no animations to be saved!");

            // check for an image file in the Animation
            if (animImgPath == null || animImgPath.Length == 0)
                throw new System.ArgumentException("There is no Image attached to this Animation!");

            if (editAnimations[currentAnimation].AnimationFrames == null || editAnimations[currentAnimation].AnimationFrames.Count == 0)
                throw new System.ArgumentException("There are no frames in this Animation to be saved!");

            SaveFileDialog sdialog = new SaveFileDialog();
            sdialog.InitialDirectory = System.IO.Path.GetFullPath(System.IO.Directory.GetCurrentDirectory() + "\\..\\..\\Game\\Animations");

            if (sdialog.ShowDialog() == DialogResult.OK)
            {

                string animFIleName = null;
                if (File.Exists(sdialog.FileName) == true)
                    animFIleName = sdialog.FileName;
                if (File.Exists(sdialog.FileName) == false)
                    animFIleName = sdialog.FileName + ".xml";

                XmlWriterSettings settings = new XmlWriterSettings();
                settings.Indent = true;
                settings.IndentChars = "\t";
                XmlWriter output = XmlWriter.Create(animFIleName, settings);

                output.WriteStartElement("Sheet");

                for (int currAnim = 0; currAnim < editAnimations.Count; ++currAnim)
                {
                    // Create Animation Element
                    output.WriteStartElement("Animation");

                    // Set String Attribute
                    output.WriteAttributeString(null, "name", null, editAnimations[currAnim].AnimationName);

                    // Set Looping Attribute
                    int isLooping = 0;
                    if (animLooping == true)
                        isLooping = 1;
                    else if (animLooping == false)
                        isLooping = 0;
                    output.WriteAttributeString(null, "isLooping", null, Convert.ToString(isLooping));

                    // Set ImageName Attribute
                    output.WriteAttributeString(null, "animImageID", null, animImgPath);

                    for (int currFrame = 0; currFrame < editAnimations[currAnim].AnimationFrames.Count; ++currFrame)
                    {
                        output.WriteStartElement("AnimFrames");
                        output.WriteAttributeString(null, "anchorPtX", null, Convert.ToString(editAnimations[currAnim].AnimationFrames[currFrame].AnchorPointX));
                        output.WriteAttributeString(null, "anchorPtY", null, Convert.ToString(editAnimations[currAnim].AnimationFrames[currFrame].AnchorPointY));
                        output.WriteAttributeString(null, "frameDuration", null, Convert.ToString(editAnimations[currAnim].AnimationFrames[currFrame].CurrFrameDuration));

                        output.WriteAttributeString(null, "rleft", null, Convert.ToString(editAnimations[currAnim].AnimationFrames[currFrame].RenderLeft));
                        output.WriteAttributeString(null, "rtop", null, Convert.ToString(editAnimations[currAnim].AnimationFrames[currFrame].RenderTop));
                        output.WriteAttributeString(null, "rright", null, Convert.ToString(editAnimations[currAnim].AnimationFrames[currFrame].RenderRight));
                        output.WriteAttributeString(null, "rbottom", null, Convert.ToString(editAnimations[currAnim].AnimationFrames[currFrame].RenderBottom));

                        output.WriteAttributeString(null, "event", null, editAnimations[currAnim].AnimationFrames[currFrame].FrameEvent);

                        output.WriteEndElement();
                    }

                    output.WriteEndElement();
                }
                output.WriteEndElement();
                output.Close();

            }

        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            editAnimations.Clear();
            UpdateAnimInfo();
            UpdateAnimFramesInfo();
            UpdateInformation();
            animImageID = -1;
        }

        private void previewAnimButton_Click(object sender, EventArgs e)
        {
            previewAnimation = true;
        }

        private void AnimImagePanel_MouseDown(object sender, MouseEventArgs e)
        {
            // drawing render rectangles with the mouse
            if (e.Button == MouseButtons.Left && e.X > 0 && e.Y > 0)
            {
                this.rectLeft = e.X;
                this.rectTop = e.Y;
                this.rectRight = this.rectLeft + 1;
                this.rectBottom = this.rectTop + 1;

                this.drawRenderRect = false;
                this.renderRightSet = false;
                this.renderBottomSet = false;
            }
            // creating anchor points with the mouse
            else if (e.Button == MouseButtons.Right && e.X > 0 && e.Y > 0)
            {
                if (editAnimations.Count != 0 && editAnimations[currentAnimation].AnimationFrames.Count != 0)
                {
                    AnchorX.Value = e.X - this.animImgOffset.X - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft;
                    AnchorY.Value = e.Y - this.animImgOffset.Y - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop;
                }

                AnchorPtX = e.X - animImgOffset.X;
                AnchorPtY = e.Y - animImgOffset.Y;

                this.drawAnchorPt = false;
            }
        }

        private void AnimImagePanel_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && e.X > 0 && e.Y > 0)
            {

            }
            else if (e.Button == MouseButtons.Right)
            {
                this.drawAnchorPt = true;
            }
        }

        private void AnimPlay_Click(object sender, EventArgs e)
        {
            this.playAnimation = true;
            animationPlayed = false;
        }

        private void AnimPause_Click(object sender, EventArgs e)
        {
            this.playAnimation = false;
            this.previewAnimation = false;
        }

        private void AnimStop_Click(object sender, EventArgs e)
        {
            this.playAnimation = false;
            this.previewAnimation = false;
            this.currentFrame = 0;

            UpdateAnimInfo();
            UpdateAnimFramesInfo();
            UpdateInformation();
        }

        private void FrameNum_ValueChanged(object sender, EventArgs e)
        {
            if (editAnimations.Count != 0)
            {
                this.currentFrame = (int)FrameNum.Value;
                if (this.currentFrame == editAnimations[currentAnimation].AnimationFrames.Count)
                    this.currentFrame = (editAnimations[currentAnimation].AnimationFrames.Count - 1);
                else if (this.currentFrame < 0)
                    this.currentFrame = 0;
                UpdateAnimFramesInfo();
                UpdateInformation();
            }
        }

        private void ValueChanged(object sender, EventArgs e)
        {
            this.AnchorPtX = (int)AnchorX.Value + this.renderLeft + animImgOffset.X;
            if (this.AnchorPtX < 0)
                this.AnchorPtX = 0;

            this.AnchorPtY = (int)AnchorY.Value + this.renderTop + animImgOffset.Y;
            if (this.AnchorPtY < 0)
                this.AnchorPtY = 0;

            this.frameDuration = (float)FrameDur.Value;
            if (this.frameDuration <= 0.0)
                this.frameDuration = 0.0f;

            this.renderLeft = (int)RenderLeft.Value;
            if (this.renderLeft < 0)
                this.renderLeft = 0;

            this.renderTop = (int)RenderTop.Value;
            if (this.renderTop < 0)
                this.renderTop = 0;

            this.renderRight = (int)RenderRight.Value;
            if (this.renderRight < 0)
                this.renderRight = 0;

            this.renderBottom = (int)RenderBottom.Value;
            if (this.renderBottom < 0)
                this.renderBottom = 0;
        }

        private void AddFrameButton_Click(object sender, EventArgs e)
        {
            AnimFrames newFrame = new AnimFrames();
            newFrame.AnchorPointX = (int)AnchorX.Value;
            newFrame.AnchorPointY = (int)AnchorY.Value;
            newFrame.CurrFrameDuration = (float)FrameDur.Value;
            newFrame.RenderLeft = (int)RenderLeft.Value;
            newFrame.RenderTop = (int)RenderTop.Value;
            newFrame.RenderRight = (int)RenderRight.Value;
            newFrame.RenderBottom = (int)RenderBottom.Value;
            newFrame.RenderRect = new Rectangle(newFrame.RenderLeft, newFrame.RenderTop,
                newFrame.RenderRight - newFrame.RenderLeft, newFrame.RenderBottom - newFrame.RenderTop);
            newFrame.FrameEvent = EventBox.Items[0].ToString();

            editAnimations[currentAnimation].AnimationFrames.Add(newFrame);
            this.currentFrame = editAnimations[currentAnimation].AnimationFrames.Count - 1;
            FrameList.Items.Add(this.currentFrame + 1);

            UpdateAnimInfo();
            UpdateAnimFramesInfo();
            UpdateInformation();
        }

        private void createFramebutton_Click(object sender, EventArgs e)
        {
            if (editAnimations == null || editAnimations.Count == 0)
                throw new System.ArgumentException("There is no Animation for this Frame to be attached to!");

            AnimFrames newFrame = new AnimFrames();
            newFrame.AnchorPointX = 0;
            newFrame.AnchorPointY = 0;
            newFrame.CurrFrameDuration = 0.0f;
            newFrame.RenderLeft = 0;
            newFrame.RenderTop = 0;
            newFrame.RenderRight = 0;
            newFrame.RenderBottom = 0;
            newFrame.RenderRect = new Rectangle(newFrame.RenderLeft, newFrame.RenderTop,
                newFrame.RenderRight - newFrame.RenderLeft, newFrame.RenderBottom - newFrame.RenderTop);
            newFrame.FrameEvent = "None";

            if (editAnimations[currentAnimation].AnimationFrames == null)
                editAnimations[currentAnimation].AnimationFrames = new List<AnimFrames>();

            editAnimations[currentAnimation].AnimationFrames.Add(newFrame);
            this.currentFrame = editAnimations[currentAnimation].AnimationFrames.Count - 1;
            FrameList.Items.Add(this.currentFrame + 1);

            EventBox.Items.Clear();
            EventBox.Items.Add(newFrame.FrameEvent);

            UpdateAnimInfo();
            UpdateAnimFramesInfo();
            UpdateInformation();
        }

        private void SaveFrameButton_Click(object sender, EventArgs e)
        {
            editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointX = (int)AnchorX.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].AnchorPointY = (int)AnchorY.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].CurrFrameDuration = (float)FrameDur.Value;

            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft = (int)RenderLeft.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop = (int)RenderTop.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRight = (int)RenderRight.Value;
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderBottom = (int)RenderBottom.Value;

            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRect = new Rectangle(
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft,
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop,
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderRight - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderLeft,
            editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderBottom - editAnimations[currentAnimation].AnimationFrames[currentFrame].RenderTop);

            editAnimations[currentAnimation].AnimationFrames[currentFrame].FrameEvent = EventBox.Items[0].ToString();
        }

        private void deleteFrameButton_Click(object sender, EventArgs e)
        {
            editAnimations[currentAnimation].AnimationFrames.Remove(editAnimations[currentAnimation].AnimationFrames[currentFrame]);
            FrameList.Items.Clear();
            for (int i = 0; i < editAnimations[currentAnimation].AnimationFrames.Count; ++i)
                FrameList.Items.Add(i + 1);

            UpdateAnimInfo();
            UpdateAnimFramesInfo();
            UpdateInformation();
        }

        public static DialogResult InputBox(string title, string promptText, ref string value)
        {
            Form form = new Form();
            Label label = new Label();
            TextBox textBox = new TextBox();
            Button buttonOk = new Button();
            Button buttonCancel = new Button();

            form.Text = title;
            label.Text = promptText;
            textBox.Text = value;

            buttonOk.Text = "OK";
            buttonCancel.Text = "Cancel";
            buttonOk.DialogResult = DialogResult.OK;
            buttonCancel.DialogResult = DialogResult.Cancel;

            label.SetBounds(9, 20, 372, 13);
            textBox.SetBounds(12, 36, 372, 20);
            buttonOk.SetBounds(228, 72, 75, 23);
            buttonCancel.SetBounds(309, 72, 75, 23);

            label.AutoSize = true;
            textBox.Anchor = textBox.Anchor | AnchorStyles.Right;
            buttonOk.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            buttonCancel.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;

            form.ClientSize = new Size(396, 107);
            form.Controls.AddRange(new Control[] { label, textBox, buttonOk, buttonCancel });
            form.ClientSize = new Size(Math.Max(300, label.Right + 10), form.ClientSize.Height);
            form.FormBorderStyle = FormBorderStyle.FixedDialog;
            form.StartPosition = FormStartPosition.CenterScreen;
            form.MinimizeBox = false;
            form.MaximizeBox = false;
            form.AcceptButton = buttonOk;
            form.CancelButton = buttonCancel;

            DialogResult dialogResult = form.ShowDialog();
            value = textBox.Text;
            return dialogResult;
        }

        private void addAnimButton_Click(object sender, EventArgs e)
        {
            string animationName = "New Animation";

            // prompt the user to put in a new animation name
            if (InputBox("Animation Name", "Input Animation Name", ref animationName) == DialogResult.OK)
            {
                Animations newAnimation = new Animations();
                newAnimation.AnimationName = animationName;
                newAnimation.AnimationFrames = null;
                newAnimation.AnimImageID = -1;
                newAnimation.IsLooping = false;

                editAnimations.Add(newAnimation);
                AnimList.Items.Clear();
                for (int i = 0; i < editAnimations.Count; ++i)
                    AnimList.Items.Add(editAnimations[i].AnimationName);


                UpdateAnimInfo();
                UpdateAnimFramesInfo();
                UpdateInformation();
            }

        }

        private void delAnimButton_Click(object sender, EventArgs e)
        {
            editAnimations.Remove(editAnimations[currentAnimation]);
            AnimList.Items.Clear();
            for (int i = 0; i < editAnimations.Count; ++i)
            {
                AnimList.Items.Add(editAnimations[i].AnimationName);
            }

            UpdateAnimInfo();
            UpdateAnimFramesInfo();
            UpdateInformation();
        }

        private void LoopingCheck_CheckedChanged(object sender, EventArgs e)
        {
            if (LoopingCheck.Checked == true)
                editAnimations[currentAnimation].IsLooping = true;
            else if (LoopingCheck.Checked == false)
                editAnimations[currentAnimation].IsLooping = false;
        }

        private void AddEvent_Click(object sender, EventArgs e)
        {
            string defaultstring = "None";
            if (InputBox("New Event", "Insert Event String", ref defaultstring) == DialogResult.OK)
            {
                editAnimations[currentAnimation].AnimationFrames[currentFrame].FrameEvent = defaultstring;
                EventBox.Items.Clear();
                EventBox.Items.Add(editAnimations[currentAnimation].AnimationFrames[currentFrame].FrameEvent);
            }
        }

        private void RemoveEvent_Click(object sender, EventArgs e)
        {
            EventBox.Items.Clear();
            EventBox.Items.Add("None");
        }

        private void AnimImagePanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && e.X > 0 && e.Y > 0)
            {
                if (e.X - animImgOffset.X > this.rectLeft)
                {
                    // set the left if moving right
                    if (e.X <= this.rectRight)
                    {
                        if (renderRightSet == true)
                            this.rectLeft = e.X;
                        else if (renderLeftSet == true)
                            this.rectRight = e.X;
                    }
                    else
                    {
                        if (renderLeftSet == false)
                        {
                            this.rectLeft = this.rectRight;
                            renderLeftSet = true;
                            renderRightSet = false;
                        }
                        this.rectRight = e.X;
                    }
                }
                else if (e.X - animImgOffset.X <= this.rectLeft)
                {
                    // set the left if moving left
                    if (renderRightSet == false)
                    {
                        this.rectRight = this.rectLeft;
                        renderRightSet = true;
                        renderLeftSet = false;
                    }
                    this.rectLeft = e.X;
                }

                if (e.Y - this.animImgOffset.Y > this.rectTop)
                {
                    // set the top if moving down
                    if (e.Y <= this.rectBottom)
                    {
                        if (renderBottomSet == true)
                            this.rectTop = e.Y;
                        else if (renderTopSet == true)
                            this.rectBottom = e.Y;
                    }
                    else
                    {
                        if (renderTopSet == false)
                        {
                            this.rectTop = this.rectBottom;
                            renderTopSet = true;
                            renderBottomSet = false;
                        }
                        this.rectBottom = e.Y;
                    }
                }
                else if (e.Y - this.animImgOffset.Y <= this.rectTop)
                {
                    // set the top if moving up
                    if (renderBottomSet == false)
                    {
                        this.rectBottom = this.rectTop;
                        renderBottomSet = true;
                        renderTopSet = false;
                    }
                    this.rectTop = e.Y;
                }

                RenderLeft.Value = rectLeft - animImgOffset.X;
                RenderTop.Value = rectTop - animImgOffset.Y;
                RenderRight.Value = rectRight - animImgOffset.X;
                RenderBottom.Value = rectBottom - animImgOffset.Y;

                this.renderLeft = rectLeft - animImgOffset.X;
                this.renderTop = rectTop - animImgOffset.Y;
                this.renderRight = rectRight - animImgOffset.X;
                this.renderBottom = rectBottom - animImgOffset.Y;

                this.drawRenderRect = true;
            }
        }

        private void AnimImagePanel_Resize(object sender, EventArgs e)
        {
            if (InitializedComponents)
                p_D3D.Resize(AnimImagePanel, AnimImagePanel.ClientSize.Width, AnimImagePanel.ClientSize.Height, false);
        }

        private void instructionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form iForm = new Form();

            RichTextBox rTBox = new RichTextBox();

            rTBox.SetBounds(0, 0, 300, 500);
            rTBox.Anchor = rTBox.Anchor | AnchorStyles.Right;

            string rTBoxString = "Instructions:\n\n"
                + "Step 1:\n"
                + "Create a new Animation by clicking on 'Create Animation' and entering a name.\n\n"
                + "Step 2:\n"
                + "Load in an Image using the File->Load Image option.\n\n"
                + "Step 3:\n"
                + "Create Frames using the 'Create Frame' option in the 'Frames' tab.\n\n"
                + "Step 4:\n"
                + "Using the mouse, use left click to create rectangles on the image and right click to assign anchor point to the image.\n\n"
                + "Step 5:\n"
                + "Add additional info (Frame Dur or Event) in the boxes provided in the tab.\n\n"
                + "Step 6:\n"
                + "Click 'Save Frame' to ensure that all frame information is saved.\n\n"
                + "Step 7:\n"
                + "Repeat Steps 3 - 6 for all frames needed.\n\n"
                + "Step 8:\n"
                + "Repeat Steps 1-7 for all Animations needed.\n\n"
                + "Step 9:\n"
                + "When done, save animation using File->Save Animation.\n\n"
                ;
            rTBox.Text = rTBoxString;

            iForm.ClientSize = new Size(300, 500);
            iForm.Controls.AddRange(new Control[] { rTBox });

            iForm.Show();
        }

        private void AnimPreviewPanel_Scroll(object sender, ScrollEventArgs e)
        {
            animPrevOffset = new Point(0, 0);
            animPrevOffset.X += AnimPreviewPanel.AutoScrollPosition.X;
            animPrevOffset.Y += AnimPreviewPanel.AutoScrollPosition.Y;

            Render();
        }

        private void AnimPreviewPanel_Resize(object sender, EventArgs e)
        {
            if (InitializedComponents)
                p_D3D.Resize(AnimPreviewPanel, AnimPreviewPanel.ClientSize.Width, AnimPreviewPanel.ClientSize.Height, false);
        }
    }
}