/*
1. 读取文本文件： 程序需要能够读取一个指定的文本文件。
2. 单词计数： 计算文本文件中总共有多少个单词。
3. 频率统计： 统计文本中每个单词出现的次数，并打印出现频率最高的前10个单词及其频率。
4. 异常处理： 程序应能妥善处理文件不存在或读取错误的情况。
*/
#include "PreComplier.h"

class TextAnalyzer {
public:
    TextAnalyzer(const std::string& filename) :filename_(filename) {

    }

    void analyze() {
        std::ifstream file(filename_);
        //file.open(filename_.c_str(), 1);
        if (!file.is_open())
        {
            std::cerr << "open file error: " << filename_ << std::endl;
            return;
        }

        std::string line;
        std::unordered_map<std::string, int> wordCount;
        int totalWords = 0;

        while (std::getline(file, line))
        {
            std::istringstream istr(line);
            std::string word;
            while (istr >> word) {
                std::string wordNew;
                for (char c : word)
                {
                    // 判断是否是字母
                    if (isalpha(c))
                    {
                        wordNew += tolower(c);
                    }
                }

                if (!wordNew.empty())
                {
                    totalWords++;
                    wordCount[wordNew]++;
                }
            }
        }

        displayResult(wordCount, totalWords);
        file.close();
    }
private:
    void displayRes(std::unordered_map<std::string, int>& wordCount, int totalWords)
    {
        std::vector<std::pair<std::string, int>> wordSort(wordCount.begin(), wordCount.end());
        std::sort(wordSort.begin(), wordSort.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
        int end = std::min(10, static_cast<int>(wordSort.size()));
        for (int i = 0; i < end; i++)
        {
            std::cout << wordSort[i].first << ": " << wordSort[i].second << std::endl;
        }

        std::cout << "totalWords: " << totalWords << std::endl;
    }

    void displayResult(std::unordered_map<std::string, int>& wordCount, int totalWords) {
        // 定义一个最小堆优先队列，存储pair类型，按second（即单词计数）排序
        auto comp = [](const auto& a, const auto& b) {
            return a.second > b.second;
            };
        std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, decltype(comp)> pq(comp);

        // 将wordCount中的元素插入到优先队列中
        for (const auto& wc : wordCount) {
            pq.push(wc);
            std::cout << pq.top().first << ": " << pq.top().second << std::endl;
            // 如果优先队列的大小超过10，弹出堆顶元素（即最小元素）
            if (pq.size() > 10) {
                pq.pop();
            }
        }

        // 输出结果，从堆顶开始输出，即频率最低的开始输出
        std::vector<std::pair<std::string, int>> topWords;
        while (!pq.empty()) {
            topWords.push_back(pq.top());
            pq.pop();
        }

        // 因为是从最小堆中取出的，所以需要反转输出
        for (auto it = topWords.rbegin(); it != topWords.rend(); ++it) {
            std::cout << it->first << ": " << it->second << std::endl;
        }

        std::cout << "totalWords: " << totalWords << std::endl;
    }
private:
    std::string filename_;
};

void writeFile()
{
    std::string inputLine;
    std::cout << "请输入内容，以'EOF'表示结束输入：";

    // 创建输出文件流对象
    std::ofstream outFile("output.txt");
    if (!outFile) {
        std::cerr << "无法创建文件。" << std::endl;
        return;
    }

    while (true) {
        std::getline(std::cin, inputLine);
        if (inputLine == "EOF") {
            break;
        }
        // 将输入的内容写入文件
        outFile << inputLine << std::endl;
    }

    outFile.close();
}