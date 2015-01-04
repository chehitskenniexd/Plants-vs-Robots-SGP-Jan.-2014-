using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WorldEditor
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

            while (theform.Looping)
            {
                theform.Update();

                theform.Render();

                Application.DoEvents();
            }
        }
    }
}
