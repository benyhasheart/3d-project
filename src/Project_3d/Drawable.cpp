#include "Drawable.h"
#include "IndexBuffer.h"

Drawable::Drawable()
	:
	mIndexBuffer(nullptr)
{
}


void Drawable::Draw(Graphics& graphic) noexcept
{
	if ( mIndexBuffer != nullptr)
	{
		graphic.GetDeviceContext()->DrawIndexed(mIndexBuffer->GetCount(), 0u, 0);
	}

}

void Drawable::ResourceBinding(Graphics& graphic) const noexcept
{
	for (auto& bind : mBinds)
	{
		bind->Bind(graphic);
	}
}

void Drawable::ResourceUpdate(Graphics& graphic) const noexcept
{
	for (auto& bind : mBinds)
	{
		bind->Update(graphic);
	}
}


void Drawable::AddBind(std::shared_ptr<Bindable> bindObject) noexcept(!IS_DEBUG)
{
	assert("don't input IndexBuffer" && typeid(*bindObject) != typeid(IndexBuffer) );
	mBinds.push_back(std::move(bindObject));
}
void Drawable::AddIndexBuffer(std::shared_ptr<class IndexBuffer> indexBuffer) noexcept(!IS_DEBUG)
{
	assert("plz input indexBuffer" && mIndexBuffer == nullptr );
	mIndexBuffer = indexBuffer.get();
	mBinds.push_back(std::move(indexBuffer));
}