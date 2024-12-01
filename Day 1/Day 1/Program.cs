// See https://aka.ms/new-console-template for more information

using System.Collections;

var inputs = await File.ReadAllLinesAsync("input.txt");

var left = new List<int>();
var right = new List<int>();

var filteredInputs = inputs.Where(line => !string.IsNullOrWhiteSpace(line) && line.Contains(' '));
foreach (var parts in filteredInputs.Select(input => input.Split(' ')))
{
    left.Add(int.Parse(parts[0]));
    right.Add(int.Parse(parts[3]));
}

PartTwo(left, right);

void PartOne(List<int> leftList, List<int> rightList)
{
    leftList.Sort();
    rightList.Sort();
    var differences = new int[leftList.Count];
    for (var i = 0; i < leftList.Count; i++)
    {
        differences[i] = Math.Abs(leftList[i] - rightList[i]);
    }
    var sum = differences.Sum();
    Console.WriteLine($"Sum of differences: {sum}");
}

void PartTwo(List<int> leftList, List<int> rightList)
{
    var values = new List<int>();
    foreach (var val in leftList)
    {
        var occurrencesInRight = rightList.Count(x => x == val);
        var product = val * occurrencesInRight;
        values.Add(product);
    }

    var sum = values.Sum();
    Console.WriteLine($"Sum of products: {sum}");
}
