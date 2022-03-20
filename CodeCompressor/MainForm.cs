using System.ComponentModel;

namespace CodeCompressor
{
    public partial class MainForm : Form
    {
        private readonly SortableBindingList<string> _exclusionList = new();
        private readonly SortableBindingList<string> _includeList = new();

        public MainForm()
        {
            InitializeComponent();
            listBoxExclusion.DataSource = _exclusionList;
            listBoxInclude.DataSource = _includeList;
        }


        /// <summary>
        /// 親フォルダ変更
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void TextBoxRootPath_TextChanged(object sender, EventArgs e)
        {
            buttonRootFolder.Enabled = false;

            _exclusionList.Clear();
            _includeList.Clear();

            if (Directory.Exists(textBoxRootPath.Text))
            {
                string filepath = textBoxRootPath.Text;
                var fileList = await Task.Run(() => Directory.GetFiles(filepath, "*.*", SearchOption.AllDirectories));

                List<string> cppExists = new() { ".c", ".cpp", ".h", ".hpp" };
                foreach (var file in fileList)
                {
                    if (cppExists.Contains(Path.GetExtension(file)))
                    {
                        _exclusionList.Add(file[(textBoxRootPath.Text.Length + 1)..]);
                    }
                }
            }

            buttonRootFolder.Enabled = true;
        }

        /// <summary>
        /// 親フォルダ選択
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ButtonRootFolder_Click(object sender, EventArgs e)
        {
            var dialog = new FolderBrowserDialog();
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                textBoxRootPath.Text = dialog.SelectedPath;
            }
        }

        /// <summary>
        /// 項目全移動
        /// </summary>
        /// <param name="source">移動元</param>
        /// <param name="destination">移動先</param>
        private static void MoveALLItem(SortableBindingList<string> source, SortableBindingList<string> destination)
        {
            foreach (var item in source)
            {
                destination.Add(item);
            }
            source.Clear();

            destination.Sort();
        }
        /// <summary>
        /// 項目移動
        /// </summary>
        /// <param name="selectedIndices">移動項目</param>
        /// <param name="source">移動元</param>
        /// <param name="destination">移動先</param>
        private static void MoveItem(ListBox.SelectedIndexCollection selectedIndices, SortableBindingList<string> source, SortableBindingList<string> destination)
        {
            List<int> indexList = new ();
            foreach(int item in selectedIndices)
            {
                indexList.Add(item);
            }
            indexList.Sort();
            indexList.Reverse();

            foreach (int index in indexList)
            {
                destination.Add(source[index]);
                source.RemoveAt(index);
            }

            source.Sort();
            destination.Sort();
        }

        /// <summary>
        /// 全ての項目を生成対象に含む
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ButtonAllInclude_Click(object sender, EventArgs e)
        {
            MoveALLItem(_exclusionList, _includeList);
        }
        /// <summary>
        /// 選択項目を生成対象に含む
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ButtonInclude_Click(object sender, EventArgs e)
        {
            MoveItem(listBoxExclusion.SelectedIndices, _exclusionList, _includeList);
        }
        /// <summary>
        /// 選択項目を対象外にする
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ButtonExclusion_Click(object sender, EventArgs e)
        {
            MoveItem(listBoxExclusion.SelectedIndices, _includeList, _exclusionList);
        }
        /// <summary>
        /// 全ての項目を対象外にする
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ButtonAllExclusion_Click(object sender, EventArgs e)
        {
            MoveALLItem(_includeList, _exclusionList);
        }

        private async void ButtonGeneration_Click(object sender, EventArgs e)
        {
            buttonGeneration.Enabled = false;

            Generation generation = new(textBoxRootPath.Text, _includeList.ToList());
            //string code = await generation.GenerationCodeAsync();
            string code = generation.GenerationCodeAsync();

            new CodeForm(code).ShowDialog();

            buttonGeneration.Enabled = true;
        }
    }
}