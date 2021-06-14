#pragma once
#include <memory>
#include <vector>

#include "Bindable.h"
#include "IGameObject.h"


class Drawable : public IGameObject
{
public:
	Drawable();
	Drawable(Drawable& src) = delete;
	virtual ~Drawable() = default;

public:
	void Draw(Graphics& graphic) noexcept ;
protected:
	void ResourceBinding(Graphics& graphic) const noexcept;
	void ResourceUpdate(Graphics& graphic) const noexcept;

protected:
	void AddBind(std::shared_ptr<Bindable> bindObject) noexcept(!IS_DEBUG);
	void AddIndexBuffer(std::shared_ptr<class IndexBuffer> indexBuffer) noexcept(!IS_DEBUG);

private:
	class IndexBuffer* mIndexBuffer;
	std::vector<std::shared_ptr<Bindable>> mBinds;
};

