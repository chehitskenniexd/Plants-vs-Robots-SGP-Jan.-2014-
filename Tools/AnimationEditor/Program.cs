using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AnimationEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Form1 theform = new Form1();

            theform.Show();

            int currTime = System.Environment.TickCount;

            while (theform.Looping)
            {
                int oldTime = currTime;
                currTime = System.Environment.TickCount;
                float fElapsedTime = ((currTime - oldTime) / 1000.0f);

                theform.Update(fElapsedTime);

                theform.Render();

                Application.DoEvents();
            }
        }
    }
}
