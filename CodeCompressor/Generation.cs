using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace CodeCompressor
{
    internal class Generation
    {
        private readonly string _parentPath;
        private readonly List<string> _fileList;
        private readonly Regex _regInclude = new(@"#( *|\t*)include( *|\t*)(<|"").*(>|"")");
        private readonly Regex _regIncludeFile = new(@"(<|"").*(>|"")");

        public Generation(string parentPath, List<string> fileList)
        {
            _parentPath = parentPath;
            _fileList = fileList;
        }

        private record SourceData
        {
            public string Path = "";
            public string Code = "";
            public int Priority = 0;
            public List<string> Dependencies = new();

            public SourceData(string path, string code, int priority, List<string> dependencies)
            {
                Path = path;
                Code = code;
                Priority = priority;
                Dependencies = dependencies;
            }
        }


        //public async Task<string> GenerationCodeAsync()
        public string GenerationCodeAsync()
        {
            Dictionary<string, SourceData?> dependent = new();

            SortedSet<string> includeLibraryList = new();

            List<string> readFileList = new(_fileList);
            for (int index = 0; readFileList.Any(); index++)
            {
                List<string> nextReadFileList = new();

                foreach (string filepath in readFileList)
                {
                    string path = Path.Combine(_parentPath, filepath);
                    //string code = await File.ReadAllTextAsync(Path.Combine(_parentPath, filepath), Encoding.GetEncoding("shift_jis"));
                    string code = File.ReadAllText(path, Encoding.GetEncoding("shift_jis"));

                    var (stdFiles, userFiles) = GetIncludeFiles(path, code);
                    stdFiles.ForEach(file => includeLibraryList.Add(file));

                    dependent[filepath] = new SourceData(filepath, code, index, userFiles);
                    SetDependent(dependent, userFiles, path, index + 1, nextReadFileList);
                }

                readFileList = nextReadFileList;
            }

            var alignmentList = GetAlignmentList(dependent);

            return GenerationCode(includeLibraryList.ToList(), alignmentList);
        }

        private void SetDependent(Dictionary<string, SourceData?> dependent, List<string> userFiles, string path, int index, List<string> nextReadFileList)
        {
            foreach (string file in userFiles)
            {
                string includeFilePath = Path.Combine(Directory.GetParent(path)!.FullName, file);
                string compPath = Path.GetFullPath(includeFilePath).Replace($"{_parentPath}\\", string.Empty);
                if (!dependent.ContainsKey(compPath))
                {
                    dependent.Add(compPath, null);
                    nextReadFileList.Add(compPath);
                }
                else
                {
                    var dep = dependent[compPath];
                    if (dep != null)
                    {
                        dep.Priority = Math.Max(dep.Priority, index + 1);
                        SetDependent(dependent, dep.Dependencies, path, index + 1, nextReadFileList);
                    }
                }
            }
        }

        private (List<string> std, List<string> user) GetIncludeFiles(string filepath, string code)
        {
            List<string> stdFiles = new();
            List<string> userFiles = new();

            MatchCollection matchList = _regInclude.Matches(code);
            foreach (Match match in matchList)
            {
                string filename = _regIncludeFile.Match(match.Value).Value[1..^1];
                string path = Path.Combine(Directory.GetParent(filepath)!.FullName, filename);

                if (File.Exists(path))
                {
                    userFiles.Add(path);
                }
                else
                {
                    stdFiles.Add(filename);
                }
            }

            return (stdFiles, userFiles);
        }

        private static List<SourceData> GetAlignmentList(Dictionary<string, SourceData?> dependent)
        {
            var codeList = dependent.ToList();

            codeList.Sort((l, r) =>
            {
                if (l.Value != null && r.Value == null) return -1;
                if (l.Value == null && r.Value != null) return 1;
                if (l.Value != null && r.Value != null)
                {
                    if (l.Value.Priority > r.Value.Priority) return -1;
                    if (l.Value.Priority < r.Value.Priority) return 1;
                }
                return string.Compare(l.Key, r.Key);
            });

            List<SourceData> sources = new();
            foreach (var source in codeList)
            {
                if (source.Value != null)
                {
                    sources.Add(source.Value);
                }
            }

            return sources;
        }

        private string GenerationCode(List<string> libraryList, List<SourceData> sourceList)
        {
            StringBuilder sb = new();

            foreach (var library in libraryList)
            {
                sb.AppendLine($"#include <{library}>");
            }

            foreach (var source in sourceList)
            {
                string code = GetRemoveIncludeCode(source.Code);
                //sb.AppendLine($"/* {source.Path}:{source.Priority} */");
                sb.AppendLine($"/*{source.Path}*/");
                sb.AppendLine(code);
            }

            return GetFormatCode(sb.ToString());
        }

        private string GetRemoveIncludeCode(string code)
        {
            string exceptCode = _regInclude.Replace(code, string.Empty);
            exceptCode = new Regex(@"# *pragma once").Replace(exceptCode, string.Empty);
            return exceptCode;
        }

        private static string GetFormatCode(string code)
        {
            var replace = (string oldValue, Regex reg, string newValue) =>
            {
                string value = oldValue;
                while (reg.IsMatch(value))
                {
                    value = reg.Replace(value, newValue);
                }
                return value;
            };

            string exceptCode = code.Replace("\t", " ");
            exceptCode = replace(exceptCode, new Regex(@"\r"), "\n");

            exceptCode = replace(exceptCode, new Regex(@"  "), " ");
            exceptCode = replace(exceptCode, new Regex(@"(\n | \n)"), "\n");
            exceptCode = replace(exceptCode, new Regex(@"//.*\n"), "\n");
            exceptCode = replace(exceptCode, new Regex(@"\n{2}"), "\n");

            exceptCode = replace(exceptCode, new Regex(@"( {|{ )"), "{");
            exceptCode = replace(exceptCode, new Regex(@"( }|} )"), "}");
            exceptCode = replace(exceptCode, new Regex(@"( <|< )"), "<");
            exceptCode = replace(exceptCode, new Regex(@"( >|> )"), ">");
            exceptCode = replace(exceptCode, new Regex(@"( \(|\( )"), "(");
            exceptCode = replace(exceptCode, new Regex(@"( \)|\) )"), ")");
            exceptCode = replace(exceptCode, new Regex(@"( \[|\[ )"), "[");
            exceptCode = replace(exceptCode, new Regex(@"( \]|\] )"), "]");

            exceptCode = replace(exceptCode, new Regex(@"( \+|\+ )"), "+");
            exceptCode = replace(exceptCode, new Regex(@"( -|- )"), "-");
            exceptCode = replace(exceptCode, new Regex(@"( /|/ )"), "/");
            exceptCode = replace(exceptCode, new Regex(@"( \*|\* )"), "*");
            exceptCode = replace(exceptCode, new Regex(@"( =|= )"), "=");
            exceptCode = replace(exceptCode, new Regex(@"( ,|, )"), ",");
            exceptCode = replace(exceptCode, new Regex(@"( :|: )"), ":");
            exceptCode = replace(exceptCode, new Regex(@"( \||\| )"), "|");
            exceptCode = replace(exceptCode, new Regex(@"( ~|~ )"), "~");
            exceptCode = replace(exceptCode, new Regex(@"( %|% )"), "%");
            exceptCode = replace(exceptCode, new Regex(@"( !|! )"), "!");
            exceptCode = replace(exceptCode, new Regex(@"( \?|\? )"), "?");
            exceptCode = replace(exceptCode, new Regex(@"( \^|\^ )"), "^");
            exceptCode = replace(exceptCode, new Regex(@"( ;|; )"), ";");
            exceptCode = replace(exceptCode, new Regex(@"( &|& )"), "&");
            exceptCode = replace(exceptCode, new Regex(@"( &=|&= )"), "&=");
            exceptCode = replace(exceptCode, new Regex(@"( #|# )"), "#");

            exceptCode = replace(exceptCode, new Regex(@"/\*\*\n\*"), "/** ");
            exceptCode = replace(exceptCode, new Regex(@"\n\*@"), " @");
            exceptCode = replace(exceptCode, new Regex(@"\n\*/"), "*/");
            exceptCode = replace(exceptCode, new Regex(@"\*\*@"), "** @");
            exceptCode = replace(exceptCode, new Regex(@" \*@"), " @");
            exceptCode = replace(exceptCode, new Regex(@"(/\*\*.*?\*/)(?!\n)"), "$1\n");

            exceptCode = replace(exceptCode, new Regex(@"({|}|<|>|\(|\)|;|:)(?!\\)\n(?!(#|/\*\*))"), "$1");
            exceptCode = replace(exceptCode, new Regex(@"(#include.*>)(?!\n)"), "$1\n");

            exceptCode = replace(exceptCode, new Regex(@"\n(?!(#|/\*\*))(.*)\n(?!(#|/\*\*))"), "\n$2");

            exceptCode = replace(exceptCode, new Regex(@"\n\*\n(?!(#|/\*\*))"), "\n");

            exceptCode = replace(exceptCode, new Regex(@"\n{2}"), "\n");

            return GetWrapCode(exceptCode);
        }
        private static string GetWrapCode(string code)
        {
            string[] splitLines = code.Split("\n");
            StringBuilder wrapCode = new();

            const int LineLength = 200;
            const int SplitLineLength = (int)(LineLength * 1.5);
            foreach (string line in splitLines)
            {
                if (line.Length < SplitLineLength)
                {
                    wrapCode.AppendLine(line);
                }
                else
                {
                    int pos = 0;
                    while (pos < line.Length)
                    {
                        if (line.Length <= pos + SplitLineLength)
                        {
                            wrapCode.AppendLine(line[pos..]);
                            break;
                        }
                        int[] splitPosList = {
                            line.IndexOf(';', pos + LineLength),
                            line.IndexOf('}', pos + LineLength),
                            line.IndexOf('{', pos + LineLength),
                            line.IndexOf(',', pos + LineLength)
                        };
                        int? splitPos = splitPosList.Where((p) => 0 <= p).Min();

                        if (splitPos != null)
                        {
                            wrapCode.AppendLine(line[pos..(splitPos.Value + 1)]);
                            pos = splitPos.Value + 1;
                            if (line[pos] == ' ')
                            {
                                pos += 1;
                            }
                        }
                        else
                        {
                            wrapCode.AppendLine(line[pos..]);
                            pos = line.Length;
                        }
                    }
                }
            }

            return wrapCode.ToString();
        }
    }
}
