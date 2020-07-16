#pragma once
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <stack>
namespace NCustom {

struct TNode {
    std::string Prefix;
    bool IsEnd = false;
    std::vector<std::unique_ptr<TNode>> Children;
};

class TRadixTree {
public:

    void PrintAsTree() {
        if (!Root) {
            std::cout << "tree is empty" << std::endl;
        }
        std::stack<std::pair<TNode*, size_t>> queue;
        queue.emplace(Root.get(), 0);
        while(!queue.empty()) {
            auto [sNode, sCapacity] = queue.top();
            queue.pop();
            std::cout << std::string(sCapacity, ' ') << "|'" << sNode->Prefix << "'" << (sNode->IsEnd ? "$" : "") << "\n";
            for(const auto& c: sNode->Children) {
                queue.emplace(c.get(), sCapacity + (sNode->Prefix.size() == 0 ? 1 : sNode->Prefix.size()));
            }
        }
    }

    void PrintItems() {
        if (!Root) {
            std::cout << "tree is empty" << std::endl;
        }
        if (Root->IsEnd) {
            std::cout << " , (this mean empty root node)" << std::endl;
        }
        std::stack<std::pair<TNode*, std::string>> queue;
        for (auto& c: Root->Children) {
            queue.emplace(c.get(), Root->Prefix);
        }
        while(!queue.empty()) {
            auto [sNode, sPrefix] = queue.top();
            queue.pop();
            if (sNode->IsEnd) {
                if (sNode->Prefix.empty()) {
                    throw std::exception();
                }
                std::cout << sPrefix << sNode->Prefix << " " << sPrefix + sNode->Prefix[0] << std::endl;
            }
            sPrefix += sNode->Prefix;
            for(const auto& c: sNode->Children) {
                queue.emplace(c.get(), sPrefix);
            }
        }
    }

    void Emplace(std::string item) {
        if(!Root) {
            Root = std::make_unique<TNode>();
            Root->IsEnd = true;
            Root->Prefix = std::move(item);
            return;
        }
        EmplaceImpl(Root, item);
    }

private:
    void EmplaceImpl(std::unique_ptr<TNode>& aNode, std::string& aItem) {
        if (aItem.empty()) {
            aNode->IsEnd = true;
            return;
        }
        if (aNode->Prefix == aItem) {
            aNode->IsEnd = true;
            return;
        }
        auto [sNodeMismatch, sItemMismatch] = std::mismatch(aNode->Prefix.begin(), aNode->Prefix.end(), aItem.begin(), aItem.end());

        if (sItemMismatch == aItem.end()) {
            // в новую записываем текущее состояние
            auto sNewNode = std::make_unique<TNode>();
            sNewNode->IsEnd = aNode->IsEnd;
            sNewNode->Prefix = std::string(sNodeMismatch, aNode->Prefix.end());
            sNewNode->Children = std::move(aNode->Children);

            //  а в текущую пишем новый префикс и добавляем потомком предыдущее состояние (которое в новом элементе)
            aNode->Prefix = aItem;
            aNode->IsEnd = true;
            aNode->Children.clear();
            aNode->Children.emplace_back(std::move(sNewNode));
            return;
        }

        if (sItemMismatch != aItem.end() && sNodeMismatch != aNode->Prefix.end()) {
            // в первого муваем текущее состояние c новым префиксом (на самом деле суффиксом)
            auto firstChild = std::make_unique<TNode>();
            firstChild->Prefix = std::string(sNodeMismatch, aNode->Prefix.end());
            firstChild->IsEnd = aNode->IsEnd;
            firstChild->Children = std::move(aNode->Children);

            // старое очищаем, пишем новый префикс и добавляем новые элементы. Элемент расщепился, так что уже не может быть конечным.
            aNode->Prefix = std::string(aItem.begin(), sItemMismatch);
            aNode->Children.clear();
            aNode->Children.emplace_back(std::move(firstChild));
            aNode->IsEnd = false;
            
            // во второго новый префикс (на самом деле суффикс)
            auto secondChild = std::make_unique<TNode>();
            secondChild->IsEnd = true;
            secondChild->Prefix = std::string(sItemMismatch, aItem.end());
            


            aNode->Children.emplace_back(std::move(secondChild));
            return;
        }

        aItem = std::string(sItemMismatch, aItem.end());
        for (auto& c: aNode->Children) {
            if (c->Prefix[0] == aItem[0]) {
                EmplaceImpl(c, aItem);
                return;
            }
        }
        aNode->Children.emplace_back(new TNode{aItem, true, {}});
    }

private:
    std::unique_ptr<TNode> Root;
};
}