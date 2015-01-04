using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Threading.Tasks;

namespace WorldEditor
{
    struct Tile
    {
        string trigger;
        string type;
        Rectangle srcRect;
        Rectangle labelRect;
        string flowDirection;

        public string FlowDirection
        {
            get { return flowDirection; }
            set { flowDirection = value; }
        }

        public Rectangle LabelRect
        {
            get { return labelRect; }
            set { labelRect = value; }
        }

        public Rectangle SrcRect
        {
            get { return srcRect; }
            set { srcRect = value; }
        }

        public string Type
        {
            get { return type; }
            set { type = value; }
        }

        public string Trigger
        {
            get { return trigger; }
            set { trigger = value; }
        }
    }
}
