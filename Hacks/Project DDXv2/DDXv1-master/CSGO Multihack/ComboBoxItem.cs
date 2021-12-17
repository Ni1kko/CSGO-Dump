namespace DDX_Multihack
{
    internal class ComboBoxItem
    {
        //[swap]
        public string Text { get; set; }
        public object Value { get; set; }
        //[/swap]

        public ComboBoxItem(string text, object value)
        {
            //[swap]
            Value = value;
            Text = text;
            //[/swap]
        }

        public override string ToString()
        {
            return Text;
        }
    }
}
